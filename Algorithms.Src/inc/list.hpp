#include <memory>
namespace alg {

template<typename T, typename A = std::allocator<T>>
class List {
public:
private:
    struct Node {
        Node *prev;
        Node *next;
        T data;
    };
public:
    List();
    ~List();

private:
    Node *head;
    Node *rear;
};

}  // namespace alg
