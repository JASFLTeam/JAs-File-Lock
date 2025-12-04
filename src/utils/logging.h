#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

enum log_nivel {
    DEBUG = 1,
    INFO = 2,
    ERR = 3
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
            */
            void registrar(const std::string msg, log_nivel nivel) {
                if (nivel == DEBUG) {
                    registrador->debug(msg);
                }
                else if (nivel == ERR) {
                    registrador->error(msg);
                }
                else {
                    registrador->info(msg);
                }
            }
        private:
            /*
            * Constructor privado
            */
            Logger() {
                registrador = spdlog::basic_logger_mt<spdlog::async_factory>(
                    "APP", "log.txt");
            }

            ~Logger() {
                spdlog::drop_all();
            }

            std::shared_ptr<spdlog::logger> registrador;
    };
}