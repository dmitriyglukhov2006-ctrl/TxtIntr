#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

// Константа для преобразования градусов в радианы
const double PI = 3.14159265358979323846;

void print_help() {
    std::cout << "Использование:\n";
    std::cout << "  calculator -o <операция> [операнды...] [-m <режим>]\n";
    std::cout << "  calculator --operation <операция> [операнды...] --mode <режим>\n";
    std::cout << "\nПоддерживаемые операции:\n";
    std::cout << "  tangent     - тангенс\n";
    std::cout << "  cotangent   - котангенс\n";
    std::cout << "\nРежимы работы:\n";
    std::cout << "  deg         - градусы (по умолчанию)\n";
    std::cout << "  rad         - радианы\n";
    std::cout << "\nКоличество операндов: 1\n";
    std::cout << "\nПримеры:\n";
    std::cout << "  calculator -o tangent 45\n";
    std::cout << "  calculator --operation cotangent --mode rad 0.785\n";
    std::cout << "  calculator -o tangent -m deg 60\n";
}

// Функция преобразования градусов в радианы
double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

// Функция для проверки, является ли угол особым случаем для тангенса
bool is_tangent_undefined(double angle_rad) {
    // Тангенс не определен при angle_rad = π/2 + π*k
    double normalized = std::fmod(angle_rad, PI);
    return std::fabs(normalized - PI/2) < 1e-10 || std::fabs(normalized + PI/2) < 1e-10;
}

// Функция для проверки, является ли угол особым случаем для котангенса
bool is_cotangent_undefined(double angle_rad) {
    // Котангенс не определен при angle_rad = π*k
    double normalized = std::fmod(angle_rad, PI);
    return std::fabs(normalized) < 1e-10;
}

// Функция тангенса с обработкой особых случаев
double tangent(double angle, bool is_radians) {
    double angle_rad = is_radians ? angle : degrees_to_radians(angle);
    
    if (is_tangent_undefined(angle_rad)) {
        if (is_radians) {
            throw std::runtime_error("Тангенс не определен для угла " + std::to_string(angle) + " радиан (π/2 + π*k)");
        } else {
            throw std::runtime_error("Тангенс не определен для угла " + std::to_string(angle) + " градусов (90° + 180°*k)");
        }
    }
    
    return std::tan(angle_rad);
}

// Функция котангенса с обработкой особых случаев
double cotangent(double angle, bool is_radians) {
    double angle_rad = is_radians ? angle : degrees_to_radians(angle);
    
    if (is_cotangent_undefined(angle_rad)) {
        if (is_radians) {
            throw std::runtime_error("Котангенс не определен для угла " + std::to_string(angle) + " радиан (π*k)");
        } else {
            throw std::runtime_error("Котангенс не определен для угла " + std::to_string(angle) + " градусов (180°*k)");
        }
    }
    
    return 1.0 / std::tan(angle_rad);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::string operation;
    std::string mode = "deg"; // Режим по умолчанию - градусы
    std::vector<double> operands;

    // Разбор аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-o" || arg == "--operation") {
            if (i + 1 < argc) {
                operation = argv[++i];
            } else {
                std::cerr << "Ошибка: не указана операция после " << arg << std::endl;
                return 1;
            }
        } else if (arg == "-m" || arg == "--mode") {
            if (i + 1 < argc) {
                mode = argv[++i];
                if (mode != "deg" && mode != "rad") {
                    std::cerr << "Ошибка: неверный режим. Используйте 'deg' или 'rad'" << std::endl;
                    return 1;
                }
            } else {
                std::cerr << "Ошибка: не указан режим после " << arg << std::endl;
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            print_help();
            return 0;
        } else {
            try {
                operands.push_back(std::stod(arg));
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: неверный формат операнда: " << arg << std::endl;
                return 1;
            }
        }
    }

    // Проверка количества операндов
    if (operands.size() != 1) {
        std::cerr << "Ошибка: требуется ровно 1 операнд, получено " << operands.size() << std::endl;
        return 1;
    }

    // Проверка наличия операции
    if (operation.empty()) {
        std::cerr << "Ошибка: не указана операция" << std::endl;
        print_help();
        return 1;
    }

    // Выполнение операции
    try {
        double result = 0.0;
        bool is_radians = (mode == "rad");
        double angle = operands[0];
        
        if (operation == "tangent") {
            result = tangent(angle, is_radians);
            std::cout << "tan(" << angle << (is_radians ? " rad" : "°") << ") = ";
        } else if (operation == "cotangent") {
            result = cotangent(angle, is_radians);
            std::cout << "cot(" << angle << (is_radians ? " rad" : "°") << ") = ";
        } else {
            std::cerr << "Ошибка: неизвестная операция: " << operation << std::endl;
            print_help();
            return 1;
        }
        
        std::cout << std::fixed << std::setprecision(6) << result << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка вычисления: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}