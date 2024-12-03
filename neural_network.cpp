#include<iostream>
#include<vector>

template<typename Derived>
struct Neurons {
public:
    template<typename T>
    void connect(T &other);
};

struct Neuron: Neurons<Neuron> {
    std::vector<Neuron*> in;
    std::vector<Neuron*> out;
    unsigned int ID;
    Neuron() {
        static int ID = 1;
        this->ID = ID++;}
    //like a vector...
    Neuron* begin() { return this; }
    Neuron* end() { return this + 1; }
};

struct NeuronLayer: std::vector<Neuron>, Neurons<NeuronLayer> {
    NeuronLayer(int number_of_neurons) {
        while (number_of_neurons-- > 0)
            emplace_back(Neuron{});
    }
};

template<typename Derived>
template<typename T>
void Neurons<Derived>::connect(T &other) {
    for(Neuron &source : *static_cast<Derived*>(this)) {
        for(Neuron &destination : other) {
            source.out.push_back(&destination);
            destination.in.push_back(&source);
        }
    }
}

template<typename Derived>
std::ostream &operator<<(std::ostream &console, Neurons<Derived> &neurons) {
    for(Neuron &current : *static_cast<Derived*>(&neurons)) {
        for(Neuron *n : current.in) {
            console << n->ID << "\t>\t" << current.ID << "*" << std::endl;
        }

        for(Neuron *n : current.out) {
            console << "" << current.ID << "*\t>\t" << n->ID << std::endl;
        }
    }
    return console;
}

int main() {
    Neuron single_neuron_1, single_neuron_2;
    NeuronLayer layer_1{1}, layer_2{2};
    single_neuron_1.connect(layer_1);
    single_neuron_1.connect(single_neuron_2);
    layer_1.connect(layer_2);
    layer_2.connect(single_neuron_2);
    std::cout << single_neuron_1 << "\n";
    std::cout << single_neuron_2 << "\n";
    std::cout << layer_1 << "\n";
    std::cout << layer_2 << "\n";
    return 0;
}