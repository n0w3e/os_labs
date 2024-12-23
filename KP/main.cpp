#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../include/DAG.h"

using json = nlohmann::json;

int main() {
    std::ifstream file("dag.json");
    json dag_json;
    file >> dag_json;

    DAG dag(dag_json);
    if (!dag.is_valid()) {
        std::cout << "DAG is invalid!" << std::endl;
        return 1;
    }

    dag.execute(4);
    return 0;
}