#include <iostream>
#include <array>
#include <limits>
#include <tuple>

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
auto run_simplex(Table<rowNum, columnNum> &inputTable) {
	// Return condition
	auto profitRow = inputTable.get_row(rowNum - 1);
	int negativeCount = 0;
	for (auto val : profitRow) negativeCount += val < 0 ? 1 : 0;
	if (negativeCount == 0) return profitRow;

	// Get most negative profit coefficient
	int mostNegativeIndex = std::get<1>(minimum<float, columnNum>(profitRow));

	// Get pivots
	auto valueColumn = inputTable.get_column(columnNum - 1);
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
	return run_simplex(inputTable);
}

int main() {

	Table<3, 6> table;
	std::array<std::array<float, 6>, 3> input {{{{1,2,2,1,0,100}}, 
												{{1,0,4,0,1,40}}, 
												{{-3,-4,-10,0,0}}}};
	table.set_table_data(input);

	print_array<float, 6> (run_simplex(table));

	return 0;
}