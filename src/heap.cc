#include <pybind11/pybind11.h>
#include <boost/heap/binomial_heap.hpp>
#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <utility>

namespace py = pybind11;

struct heap_data;
using Data = std::pair<int, int>;
using PQ = boost::heap::binomial_heap<heap_data>;
using Handle = PQ::handle_type;
using LUT = std::unordered_map<Data, Handle, boost::hash<std::pair<int, int>>>;

struct heap_data {
    Handle handle;
    Data data;
    int count;

    heap_data(int first, int second, int c) 
      : data(first, second), count(c) {
    }
    bool operator<(heap_data const & rhs) const {
        return count < rhs.count;
    }
};

struct MyPQ {
    MyPQ() : pq(), map() {}
    bool has(int first, int second) {
        return map.find(std::make_pair(first, second)) != map.end();
    }
    void push(int first, int second, int count) { 
        Handle h = pq.push(heap_data(first, second, count)); 
        map.insert(std::make_pair(std::make_pair(first, second), h));
    }
    void erase(int first, int second) {
        auto p = std::make_pair(first, second);
        Handle h = map[p];
        pq.erase(h);
        map.erase(p);
    }
    void pop() { map.erase(pq.top().data); pq.pop(); }
    const Data& top() { return pq.top().data; }
    const int& topCount() { return pq.top().count; }
    void update(int first, int second, int count) {
        Handle h = map[std::make_pair(first, second)];
        //pq.update(h, count);
    }
    void increment(int first, int second) {
        auto it = map.find(std::make_pair(first, second));
        if (it == map.end()) {
            push(first, second, 1);
        } else {
            Handle h = it->second;
            ++(*h).count;
            pq.increase(h);
        }
    }
    void decrement(int first, int second) {
        auto it = map.find(std::make_pair(first, second));
        if (it == map.end()) {
        } else {
            Handle h = it->second;
            --(*h).count;
            if ((*h).count <= 0) {
                map.erase((*h).data);
                pq.erase(h);
            } else {
                pq.decrease(h);
            }
        }
    }
    void add(int first, int second, int count) {
        auto it = map.find(std::make_pair(first, second));
        if (it == map.end()) {
            push(first, second, count);
        } else {
            Handle h = it->second;
            (*h).count += count;
            pq.increase(h);
        }
    }
    void sub(int first, int second, int count) {
        auto it = map.find(std::make_pair(first, second));
        if (it == map.end()) {
        } else {
            Handle h = it->second;
            (*h).count -= count;
            if ((*h).count <= 0) {
                map.erase((*h).data);
                pq.erase(h);
            } else {
                pq.decrease(h);
            }
        }
    }
    const int& lookup(int first, int second) {
        return (*map[std::make_pair(first, second)]).count;
    }
    PQ pq;
    LUT map;
};

PYBIND11_MODULE(heappy, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    py::class_<MyPQ>(m, "PQ")
        .def(py::init<>())
        .def("push",      &MyPQ::push)
        .def("erase",     &MyPQ::erase)
        .def("pop",       &MyPQ::pop)
        .def("top",       &MyPQ::top)
        .def("topCount",  &MyPQ::topCount)
        .def("update",    &MyPQ::update)
        .def("increment", &MyPQ::increment)
        .def("decrement", &MyPQ::decrement)
        .def("add",       &MyPQ::add)
        .def("sub",       &MyPQ::sub)
        .def("has",       &MyPQ::has)
        .def("lookup",    &MyPQ::lookup);
}

