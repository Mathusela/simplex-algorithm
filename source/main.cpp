#include <iostream>
#include <array>

template <typename T, int len>
void print_array(std::array<T, len> arrayIn) {
  for (auto val : arrayIn) std::cout << val << ", "; std::cout << std::endl;
}

template <int rowNum, int columnNum>
class Table {
private:
  std::array<std::array<float, columnNum>, rowNum> m_tableArray;
public:
  void set_table_data(std::array<std::array<float, columnNum>, rowNum> dataIn) {
    m_tableArray = dataIn;
  }

  std::array<float, columnNum> get_row(int i) {
    std::array<float, columnNum> outputArray;
    outputArray = m_tableArray[i];
    return outputArray;
  }

  std::array<float, rowNum> get_column(int i) {
    std::array<float, rowNum> outputArray;
    for (int j=0; j<rowNum; j++) {
      outputArray[j] = m_tableArray[j][i];
    }
    return outputArray;
  }
};

int main() {

  Table<3, 3> table;
  std::array<std::array<float, 3>, 3> input {{{{1,2,3}}, 
                                              {{5,3,4}}, 
                                              {{3,4,5}}}};
  table.set_table_data(input);

  print_array<float, 3>(table.get_row(0));
  print_array<float, 3>(table.get_row(1));
  print_array<float, 3>(table.get_row(2));

  print_array<float, 3>(table.get_column(0));

  return 0;
}