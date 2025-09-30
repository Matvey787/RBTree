# Red-Black Tree

## Правила

1) Каждый узел либо красный, либо чёрный.
2) Корень всегда чёрный.
3) Все листья чёрные.
4) Оба потомка красного узла — чёрные.
5) Любой путь от узла к его потомкам содержит одинаковое число чёрных узлов (чёрная высота).

## Алгоритм

1) Вставка нового узла.
2) Если родительский узел красный, то:
   a) Если дядя красный, то:
      i) Переокрашиваем родителя и дядю в чёрный, а дедушку в красный.
      ii) Продолжаем с дедушкой.
   b) Если дядя чёрный, то:
      i) Если новый узел — правый потомок, то выполняем левый поворот.
      ii) Выполняем правый поворот на дедушке и меняем цвета.

cmake -G Ninja -S . -B build
cmake --build build

cmake -G Ninja -S . -B build -DCMAKE_CXX_COMPILER=clang++
cmake --build build
./build/rbt

cmake -G Ninja -S . -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS="-stdlib=libc++ -fexperimental-library"
cmake --build build

dot -Tsvg graphviz/tree.dot -o graphviz/tree.svg