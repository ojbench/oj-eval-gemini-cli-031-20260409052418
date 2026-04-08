#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <vector>
#include <memory>

class pylist {
private:
    enum Type { INT, LIST };
    Type type;
    int int_val;
    std::shared_ptr<std::vector<pylist>> list_val;

public:
    pylist() : type(LIST), int_val(0), list_val(std::make_shared<std::vector<pylist>>()) {}
    pylist(int v) : type(INT), int_val(v), list_val(nullptr) {}

    operator int() const { return int_val; }

    void append(const pylist &x) {
        if (type == LIST) {
            list_val->push_back(x);
        }
    }

    pylist pop() {
        if (type == LIST && !list_val->empty()) {
            pylist back = list_val->back();
            list_val->pop_back();
            return back;
        }
        return pylist();
    }

    pylist &operator[](size_t i) {
        if (type == LIST) {
            return (*list_val)[i];
        }
        static pylist dummy;
        return dummy;
    }

    friend std::ostream &operator<<(std::ostream &os, const pylist &ls) {
        static std::vector<void*> visited;
        if (ls.type == INT) {
            os << ls.int_val;
        } else {
            void* ptr = ls.list_val.get();
            for (void* v : visited) {
                if (v == ptr) {
                    os << "[...]";
                    return os;
                }
            }
            
            struct VisitedGuard {
                std::vector<void*>& v;
                VisitedGuard(std::vector<void*>& vec, void* p) : v(vec) { v.push_back(p); }
                ~VisitedGuard() { v.pop_back(); }
            } guard(visited, ptr);

            os << "[";
            for (size_t i = 0; i < ls.list_val->size(); ++i) {
                if (i > 0) os << ", ";
                os << (*ls.list_val)[i];
            }
            os << "]";
        }
        return os;
    }
};

#endif //PYLIST_H
