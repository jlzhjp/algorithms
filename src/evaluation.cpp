#include "evaluation.hpp"

#include <cmath>
#include <sstream>

#include "stack.hpp"

namespace alg {

double evaluate(const std::string &expr) {
    std::istringstream t_stream(expr);
    Stack<std::string> ops;
    Stack<double> vals;
    auto is_operator = [](const std::string &token) {
        return token == "+" || token == "-" || token == "*" || token == "/" ||
               token == "sqrt";
    };

    std::string token;
    while (t_stream >> token) {
        if (token == "(") {
            continue;
        } else if (is_operator(token)) {
            ops.push(token);
        } else if (token == ")") {
            std::string op = ops.pop();
            double v = vals.pop();
            if (op == "+") {
                v = vals.pop() + v;
            } else if (op == "-") {
                v = vals.pop() - v;
            } else if (op == "*") {
                v = vals.pop() * v;
            } else if (op == "/") {
                v = vals.pop() / v;
            } else if (op == "sqrt") {
                v = std::sqrt(v);
            }
            vals.push(v);
        } else {
            vals.push(std::stod(token));
        }
    }
    return vals.pop();
}

}  // namespace alg
