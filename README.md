# Memetic Algorithm with explicit management of diversity for the one-sided crossing minimization problem.
This is a fork of the winner metaheuristic of the **PACE Challenge 2024 - Heuristic track** developed by CIMAT_Team [1].

It includes additional features and components needed for an in-depth analysis of the proposal.

## System requirements
The implementation was written and designed to be run in a Linux environment. Nevertheless, it should be capable to be compiled and run in other operating systems with minor modifications.

The following instructions have been tested in Debian 13.

## Execution
In order to compile it, just enter the `src/` folder and execute `make`. The only dependency is g++.
To execute the solver, a json configuration file is required. The configuration file must contain the following parameters:

|  Parameter               | Description                                                                  |
|--------------------------|------------------------------------------------------------------------------|
| `inputFile`              | (string) Path of the input graph (PACE2024 format).                          |
| `outputFile`             | (string) Path to output file to save results.                                |
| `timeLimit`              | (double) Time limit of execution in seconds.                                 |
| `replacement`            | (string) Replacement operator to be used (`BNP`, `GEN`, `ELIT`, or `TRUN`).  |
| `perturbations`          | (list ints) Perturbations to be used in local search (`0`, `1`, `2`).        |
| `seed`                   | (int) Seed for random numbers generation.                                    |
| `tracing`                | (bool) Flag to enable diversity and fitness tracing.                         |
| `traceCount`             | (int) Amount of tracings to perform among the algorithm executions.          |
| `reduction`              | (bool) Enables reduction of the input graph.                                 |

An example of this configuration is the following (`examples/1.conf.json`)
```json
{
    "inputFile": "/home/yourpath/CIMAT_Team/INSTANCIAS/12.gr",
    "outputFile": "/home/yourpath/CIMAT_Team/1.sol.json",
    "timeLimit": 300,
    "replacement": "ELIT",
    "perturbations": [0, 1, 2],
    "seed": 1,
    "tracing": true,
    "traceCount": 100,
    "reduction": true
}
```

## Example of running for `input1.txt` (set the paths accordingly)
`./src/solver "/home/yourpath/input1.json"`

## Output
Once the program is executed, the output json file will be created in the path described at `outputFile` with the result of the execution. This json file contains the following fields:

|  Parameter               | Description                                                                                  |
|--------------------------|----------------------------------------------------------------------------------------------|
| `input`                  | (json) The input configuration of the experiment.                                            |
| `ngen`                   | (int) Number of applied iterations of the algorithm.                                         |
| `reduced`                | (bool) `true` if the input graph was reduced, `false` otherwise.                             |
| `lsCallsCount`           | (int) Number of total local search calls.                                                    |
| `runtime`                | (double) Total running time of the execution.                                                |
| `isBig`                  | (bool) `false` if memetic algorithm was applied.                                             |
| `solution`               | (int array) Best-found solution.                                                             |
| `trace`                  | (array) Trace records of fitness, diversity, expected diversity, and elapsed running time.   |


## References
[1] C. Segura, L. Lugo, G. Miranda, and E. D. Serrano Cárdenas, “PACE Solver Description: CIMAT_Team,” in 19th International Symposium on Parameterized and Exact Computation (IPEC 2024), É. Bonnet and P. Rzą\.zewski, Eds., in Leibniz International Proceedings in Informatics (LIPIcs), vol. 321. Dagstuhl, Germany: Schloss Dagstuhl – Leibniz-Zentrum für Informatik, 2024, p. 31:1-31:4. doi: 10.4230/LIPIcs.IPEC.2024.31.
