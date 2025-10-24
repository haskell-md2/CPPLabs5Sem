#include <string>
#include <vector>

class IOperation{

    public:
        virtual ~IOperation() = default;
        virtual std::string getName() const = 0;
        virtual float execute(const std::vector<float>& args) = 0;
        virtual size_t getArgumentCount() const = 0;
};