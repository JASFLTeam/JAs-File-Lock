#ifndef resul_task_H
#define resul_task_H
#include <optional>
#include <string>

struct resul_task{

    bool exito; //false indica error fatal

    std::optional<int> codi_erro;

    std::optional<std::string> mens_erro;

};

/*
* Habilita que qt lo reconozca como meta-type
*/
Q_DECLARE_METATYPE(resul_task)

#endif 
