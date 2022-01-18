#include <iostream>
#include <array>
#include <limits>
#include <tuple>
#include <string>
#include<string.h>

template <typename T, int len>
void print_array(std::array<T, len> arrayIn) {
	for (auto val : arrayIn) std::cout << val << ", "; std::cout << std::endl;
}

template <typename T, typename F, int len>
void print_tuple_array(std::array<std::tuple<T, F>, len> arrayIn) {
	for (auto val : arrayIn) std::cout << "{" << std::get<0>(val) << ":" << std::get<1>(val) << "}" << ", "; std::cout << std::endl;
}

template <int rowNum, int columnNum>
class Table {
private:
	std::array<std::array<float, columnNum>, rowNum> m_tableArray;
public:
	void set_table_data(std::array<std::array<float, columnNum>, rowNum> dataIn) {
		m_tableArray = dataIn;
	}

	void set_row(int i, std::array<float, columnNum> inputArray) {
		m_tableArray[i] = inputArray;
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

	float get_coordinate(int x, int y) {
		return m_tableArray[y][x];
	}

};

template <typename t_arrayType, int size>
auto minimum(std::array<t_arrayType, size> arrayIn) {
	t_arrayType minValue = std::numeric_limits<t_arrayType>::max();
	int minIndex = 0;
	for (int i=0; i<size; i++) {
		minIndex = arrayIn[i] < minValue ? i : minIndex;
		minValue = arrayIn[i] < minValue ? arrayIn[i] : minValue;
	}
	return std::make_tuple(minValue, minIndex);
}

template <typename t_arrayType, int size>
auto divide_array_by_value(std::array<t_arrayType, size> arrayIn, float value) {
	std::array<t_arrayType, size> outputArray;
	for (int i=0; i<size; i++) {
		outputArray[i] = arrayIn[i] / value;
	}
	return outputArray;
}

template <typename t_arrayType, int size>
auto multiply_array_by_value(std::array<t_arrayType, size> arrayIn, float value) {
	std::array<t_arrayType, size> outputArray;
	for (int i=0; i<size; i++) {
		outputArray[i] = arrayIn[i] * value;
	}
	return outputArray;
}

template <typename t_arrayType, int size>
auto add_arrays(std::array<t_arrayType, size> arrayIn1, std::array<t_arrayType, size> arrayIn2) {
	std::array<t_arrayType, size> outputArray;
	for (int i=0; i<size; i++) {
		outputArray[i] = arrayIn1[i] + arrayIn2[i];
	}
	return outputArray;
}

template <int rowNum, int columnNum>
auto run_simplex(Table<rowNum, columnNum> &inputTable, std::string varLabels, std::array<int, (int)rowNum> rowLabels) {
	// Return condition
	auto profitRow = inputTable.get_row(rowNum - 1);
	auto valueColumn = inputTable.get_column(columnNum - 1);
	int negativeCount = 0;
	for (auto val : profitRow) negativeCount += val < 0 ? 1 : 0;

	if (negativeCount == 0) {
		std::string included = "";
		std::array<std::tuple<std::string, float>, columnNum> output;
		for (int i=0; i<rowNum; i++) {
			auto label = varLabels[rowLabels[i]];
			auto value = valueColumn[i];
			output[i] = std::make_tuple(label, value);
			included += label;
		}
		int count = 0;
		for (int i=0; i<columnNum; i++) {
			if (included.find(varLabels[i]) == std::string::npos) {
				output[rowNum + count] = std::make_tuple(varLabels[i], 0.0);
				count++;
			}
		}
		return output;
	}

	// Get most negative profit coefficient
	int mostNegativeIndex = std::get<1>(minimum<float, columnNum>(profitRow));

	// Get pivots
	std::array<float, rowNum> pivots;
	for (int i=0; i<rowNum; i++) {
		pivots[i] = valueColumn[i] / inputTable.get_coordinate(mostNegativeIndex, i);
	}
	pivots[rowNum - 1] = std::numeric_limits<float>::max();
	auto minimumPivotIndex = std::get<1>(minimum<float, rowNum>(pivots));

	// Switch out rows
	auto originalRow = inputTable.get_row(minimumPivotIndex);
	auto newRow = divide_array_by_value<float, columnNum>(originalRow, inputTable.get_coordinate(mostNegativeIndex, minimumPivotIndex));
	inputTable.set_row(minimumPivotIndex, newRow);
	auto newPivotRow = newRow;

	// auto originalRowLabel = varLabels[rowLabels[minimumPivotIndex]];
	// auto newRowLabel = varLabels[mostNegativeIndex];
	rowLabels[minimumPivotIndex] = mostNegativeIndex;

	// Row operations
	for (int i=0; i<rowNum; i++) {
		if (i == minimumPivotIndex) {
			continue;
		} else {
			originalRow = inputTable.get_row(i);
			newRow = add_arrays<float, columnNum>(originalRow, multiply_array_by_value<float, columnNum>(newPivotRow, -1 * inputTable.get_coordinate(mostNegativeIndex, i)));
			inputTable.set_row(i, newRow);
		}
	}

	// Call recursively
	return run_simplex(inputTable, varLabels, rowLabels);
}

template <int rowNum, int columnNum>
auto begin_simplex(Table<rowNum, columnNum> &inputTable, std::string varLabels) {
	std::array<int, rowNum> rowLabels;
	for (int i=0; i<rowNum; i++) {
		rowLabels[i] = columnNum - rowNum + i;
	}
	return run_simplex<rowNum, columnNum>(inputTable, varLabels, rowLabels);
}

int main() {

	Table<5, 9> table;
	std::array<std::array<float, 9>, 5> input {{{{1,4,3,1,1,0,0,0,95}}, 
												{{2,1,2,3,0,1,0,0,67}}, 
												{{1,3,2,2,0,0,1,0,75}},
												{{3,2,1,2,0,0,0,1,72}},
												{{-4,3,-2,-3,0,0,0,0,0}}}};
	table.set_table_data(input);

	print_tuple_array<std::string, float, 9>( begin_simplex<5, 9>(table, "xyzwrstuP") );

	return 0;
}