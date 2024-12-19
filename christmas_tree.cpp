#include <iostream>

/* Wykorzystjąc TMP (Template Meta Programming) narysować świąteczne drzewko.
   Zabronione jest korzystanie z pętli, instrukcji warunkowych itp.
*/

template<bool C, int Level, int MaxLevels>
class IsInRange {};

template<int Level, int MaxLevels>
class IsInRange<true, Level, MaxLevels> {
public:
    static inline void f() {
        std::string padding(MaxLevels / 2 - (Level + 1) / 2, ' ');
        std::string stars(Level + 1, '*');
        std::cout << padding << stars << padding << std::endl;   
    }
};

template<int Level, int MaxLevels>
class IsInRange<false, Level, MaxLevels> {
public:
    static inline void f() {}
};

template<bool C, int Level, int MaxLevels>
class IsEvenRow {};

template<int Level, int MaxLevels>
class IsEvenRow<true, Level, MaxLevels> {
public:
    static inline void f() {
        IsInRange<Level < MaxLevels, Level, MaxLevels>::f();
    }
};

template<int Level, int MaxLevels>
class IsEvenRow<false, Level, MaxLevels> {
public:
    static inline void f() {
        std::cout << std::endl;
    }
};

template<int Size, int MaxLevels>
class PrintTrunk {
public:
    static inline void f() {
        std::string padding(MaxLevels / 2, ' ');
        std::cout << padding << '*' << padding << std::endl;
        PrintTrunk<Size - 1, MaxLevels>::f();
    }
};

template<int MaxLevels>
class PrintTrunk<0, MaxLevels> {
public:
    static inline void f() {}
};

template<int Level, int MaxLevels, bool IsEven>
class PrintTree {
public:
    static inline void f() {}
};

template<int Level, int MaxLevels>
class PrintTree<Level, MaxLevels, false> {
public:
    static inline void f() {
        PrintTrunk<2, MaxLevels>::f();
    }
};

template<int Level, int MaxLevels>
class PrintTree<Level, MaxLevels, true> {
public:
    static inline void f() {
        IsEvenRow<Level % 2 == 0, Level, MaxLevels>::f();
        PrintTree<Level + 1, MaxLevels, (Level + 1) < MaxLevels>::f();
    }
};

template<int Levels>
class ChristmasTree {
public:
    static inline void print() {
        PrintTree<0, 2 * Levels - 1, true>::f();
    }
};

int main()
{
    //printChristmasTree(5);
    ChristmasTree<2>::print();
    return 0;
}

/* Oczekiwane wyjście:
    *
    
   ***
   
  *****
  
 *******
 
*********

*/