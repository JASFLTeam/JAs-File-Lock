#include <task.h>
#include <../crypt/argon_wrapper.h>
#include <logging.h>

namespace task {

    /*
    * Ejemplo de Tarea
    */
    class UsarArgon2 : public ut::Tarea {
        public:
            UsarArgon2(QWidget *parent = nullptr);
        private:
            void ejecutar(const datos_task& datos_inicial,
                resul_task& result,
                datos_task& salida) override {

                //Temp
                CryptoPP::SecByteBlock p;

                ut::argon2 argon_algoritmo;
                /*
                * Importante realizar acciones que podrían fallar adentro
                * del  bloque try catch. Con la finalidad de evitar
                * crasheo del programa y poder gestionar errores imprevistas
                */
                try {
                    /*                   NOTA: Es obligatorio usar estas mismas interfaces, para devolver y recibir
                    * Dato de entrada
                    */
                    const CryptoPP::SecByteBlock& clave = std::get<CryptoPP::SecByteBlock>(datos_inicial.resul_dt);
                    /*
                    * Dato de salida
                    */
                    salida.resul_dt = CryptoPP::SecByteBlock(32);
                    CryptoPP::SecByteBlock& _salida = std::get<CryptoPP::SecByteBlock>(salida.resul_dt);

                    if (datos_inicial.pro == HASH) {
                        p = datos_inicial.sec_c;
                    }

                    /*
                    * Lógica de gestión
                    */
                    int resultado = argon_algoritmo.hash_2id(clave, _salida, p, (argon_op)*datos_inicial.pro);
                    if (resultado == ARGON2_OK) {
                        ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Hash generado exitosamente",INFO);
                        /*
                        * Si es true no hace falta definir el resto
                        */
                        salida.clave = p;
                        result.exito = true;
                    }
                    else {
                        ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Hash generado, pero con problema",ERR);
                        ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::to_string(resultado),ERR);
                        result.exito = false;
                        result.codi_erro = resultado;
                    }
                }
                /*
                * Por si fallas imprevistas
                */
                catch (std::exception& err) {
                    //otros
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,err.what(),ERR);
                    result.exito = false;
                    result.mens_erro = err.what();
                    result.codi_erro = 1; //ejemplo
                }
            }
    };
    /*
    * Fín
    */
}
