#ifndef LOGGING_H
#define LOGGING_H
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

/*
* Detectar build
*/
#ifdef DEBUG_ON
    static const bool IS_DEBUG = true;
#else
    static const bool IS_DEBUG = false;
#endif

enum log_nivel {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERR = 4
};

namespace ut {
    class Logger {
        public:
            /*
            * Instancia única
            */
            static Logger& Instancia() {
                static Logger instancia_a;
                return instancia_a;
            }
            /*
            * Funcion que en función del nivel DEBUG, INFO, ERROR lo registra de distinta manera
            * Uso: ut::Logger::Instancia().registrar("msg",INFO);
            *
            * Macros importantes: __FILE__, __LINE__ y __FUNCTION__
            */
            void registrar(
                const char* archivo, int linea, const char* func, const std::string msg, log_nivel nivel) {
                /*
                * En caso de no ser debug la build Nota:-No el nivel de log
                */
                if (!IS_DEBUG) {
                    if (nivel == DEBUG) {
                        registrador->debug(msg);
                    }
                    else if (nivel == ERR) {
                        registrador->error(msg);
                    }
                    else if (nivel == WARN) {
                        registrador->warn(msg);
                    }
                    else {
                        registrador->info(msg);
                    }
                }
                /*
                * Cuando es debug Nota:-No confundir con el nivel debug
                */
                else {
                    auto loc = spdlog::source_loc{archivo, linea, func};

                    if (nivel == DEBUG) {
                        registrador->log(loc, spdlog::level::debug, msg);
                    }
                    else if (nivel == ERR) {
                        registrador->log(loc, spdlog::level::err, msg);
                    }
                    else if (nivel == WARN) {
                        registrador->log(loc, spdlog::level::warn, msg);
                    }
                    else {
                        registrador->log(loc, spdlog::level::info, msg);
                    }
                }
            }
        private:
            /*
            * Constructor privado
            */
            Logger() {
                registrador = spdlog::basic_logger_mt<spdlog::async_factory>(
                    "APP", "log.txt", true); //true es para iniciar un nuevo log sin preservar el anterior

                /*
                * Formato dependiente del tipo de build Debug o Release
                */
                if (IS_DEBUG) {
                    /* Ejemplo de salida:
                    * [14-11-25 09:30:45:123] [info] [app] "contraseña renovada!"
                    */
                    registrador->set_pattern("[%d-%m-%C %H:%M:%S:%e] [%l] [%n] \"%v\" [%s:%# %!]");
                }
                else {
                    registrador->set_pattern("[%d-%m-%C %H:%M:%S:%e] [%l] [%n] \"%v\"");
                }

            }

            ~Logger() {
                spdlog::shutdown();
            }

            std::shared_ptr<spdlog::logger> registrador;
    };
}

#endif
