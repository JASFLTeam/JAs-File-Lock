#ifndef DATOS_TASK_H
#define DATOS_TASK_H
#include <string>
#include <vector>
#include <variant>
#include <optional>

using reu_var = std::variant <

    bool,
    std::string,
    std::vector<std::string>,
    std::monostate

>;

struct datos_task{

    reu_var resul_dt;
    std::optional<int> pro;

}; 


#endif 

