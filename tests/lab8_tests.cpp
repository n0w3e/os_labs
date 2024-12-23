#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "../include/DAG.h"
#include "../include/Job.h"
#include <sstream>

using json = nlohmann::json;

TEST(DAGTest, ValidDAG) {
    std::stringstream ss;
    ss << R"({
        "jobs": [
            {
                "id": "job1",
                "dependencies": []
            },
            {
                "id": "job2",
                "dependencies": ["job1"]
            },
            {
                "id": "job3",
                "dependencies": ["job1"]
            },
            {
                "id": "job4",
                "dependencies": ["job2", "job3"]
            }
        ]
    })";

    json dag_json;
    ss >> dag_json;

    DAG dag(dag_json);
    EXPECT_TRUE(dag.is_valid());
}

TEST(DAGTest, CyclicDAG) {
    std::stringstream ss;
    ss << R"({
        "jobs": [
            {
                "id": "job1",
                "dependencies": ["job4"]
            },
            {
                "id": "job2",
                "dependencies": ["job1"]
            },
            {
                "id": "job3",
                "dependencies": ["job1"]
            },
            {
                "id": "job4",
                "dependencies": ["job2", "job3"]
            }
        ]
    })";

    json dag_json;
    ss >> dag_json;

    DAG dag(dag_json);
    EXPECT_FALSE(dag.is_valid());
}

TEST(DAGTest, NoStartJobsDAG) {
    std::stringstream ss;
    ss << R"({
        "jobs": [
            {
                "id": "job1",
                "dependencies": ["job2"]
            },
            {
                "id": "job2",
                "dependencies": ["job3"]
            },
            {
                "id": "job3",
                "dependencies": ["job4"]
            },
            {
                "id": "job4",
                "dependencies": ["job1"]
            }
        ]
    })";

    json dag_json;
    ss >> dag_json;

    DAG dag(dag_json);
    EXPECT_FALSE(dag.is_valid());
}

TEST(DAGTest, MultipleComponentsDAG) {
    std::stringstream ss;
    ss << R"({
        "jobs": [
            {
                "id": "job1",
                "dependencies": []
            },
            {
                "id": "job2",
                "dependencies": ["job1"]
            },
            {
                "id": "job3",
                "dependencies": []
            },
            {
                "id": "job4",
                "dependencies": ["job3"]
            }
        ]
    })";

    json dag_json;
    ss >> dag_json;

    DAG dag(dag_json);
    EXPECT_TRUE(dag.is_valid());
}

TEST(DAGTest, SingleJobDAG) {
    std::stringstream ss;
    ss << R"({
        "jobs": [
            {
                "id": "job1",
                "dependencies": []
            }
        ]
    })";

    json dag_json;
    ss >> dag_json;

    DAG dag(dag_json);
    EXPECT_TRUE(dag.is_valid());
}