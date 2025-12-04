#ifndef resul_task_H
#define resul_task_H
#include <optional>
#include <string>

struct resul_task{

    bool exito;

    std::optional<int> codi_erro;

    std::optional<std::string> mens_erro;

};


#endif 
