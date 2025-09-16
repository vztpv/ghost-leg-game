
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

struct Line {
	int level;
	int left;
	int right; // right == left + 1

	bool operator<(const Line& other) const {
		if (level != other.level) {
			return level < other.level;
		}
		return left < other.left;
	}
};

// 오름차순.
void bubble_sort(std::vector<int>& vec, std::set<Line>& lines) {
	for (uint64_t i = vec.size() - 1; i > 0; --i) {
		for (uint64_t j = 0; j < i; ++j) {
			if (vec[j] > vec[j + 1]) {
				std::swap(vec[j], vec[j + 1]);
				
				Line line{ vec.size() - 1 - i, j, j + 1};
				lines.insert(line);
			}
		}
	}
}

void change_line(const std::vector<Line>& lines, std::vector<Line>& result) {
	if (lines.empty()) {
		return;
	}

	result.push_back(lines[0]);
	
	int level = lines[0].level;

	for (uint64_t i = 1; i < lines.size(); ++i) {
		if (lines[i - 1].level == lines[i].level && lines[i - 1].right == lines[i].left) {
			++level;
			Line line{ level, lines[i].left, lines[i].right };
			result.push_back(line);
		}
		else {
			// lines[i].level - lines[i - 1].level == 0 or 1
			if (lines[i - 1].level == lines[i].level && lines[i - 1].right >= lines[i].right) {
				++level;
			}
			if (lines[i - 1].level < lines[i].level) {
				++level;
			}
			Line line{ level, lines[i].left, lines[i].right };
			result.push_back(line);
		}
	}
}

void draw_ladder(const std::vector<int>& result, const std::vector<Line>& line_result) {
	uint64_t ladder_size = result.size();

	for (uint64_t i = 0; i < ladder_size; ++i) {
		std::cout << i << "\t";
	}
	std::cout << "\n";

	uint64_t now_level = line_result.empty() ? 0 : line_result[0].level;
	uint64_t min_level = now_level;
	uint64_t max_level = line_result.empty() ? now_level : line_result.back().level;

	std::vector<std::set<Line>> table(max_level - now_level + 1);

	for (uint64_t i = 0; i < line_result.size(); ++i) {
		table[line_result[i].level - min_level].insert(line_result[i]);
	}
	
	std::reverse(table.begin(), table.end());

	for (uint64_t i = 0; i < table.size(); ++i) {
		int k = 0;	
		auto iter = table[i].begin();
		for (; k < ladder_size; ++k) {
			if (k < iter->left) {
				std::cout << "|\t";
			}
			else if (k == iter->left && iter != table[i].end()) {
				std::cout << "|-------|\t";
				++k;
				++iter;
			}
			else {
				std::cout << "|\t";
			}
		}
		std::cout << "\n";
	}

	for (uint64_t i = 0; i < ladder_size; ++i) {
		std::cout << result[i] << "\t";
	}
	std::cout << "\n";
}

int main(void)
{
	//srand(time(nullptr));
	std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6 }; // start from 0
	std::set<Line> lines;

	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(vec.begin(), vec.end(), g);
		//std::reverse(vec.begin(), vec.end());
		//std::random_shuffle(vec.begin(), vec.end());
		vec = { 5, 3, 6, 2, 4, 1, 0 };
		// 2, 4, 0, 6, 1, 
	}

	std::cout << "\n";
	
	{
		std::vector<int> vec2 = vec;
		bubble_sort(vec2, lines);
		std::vector<Line> line_result;
		std::vector<Line> lines_vec(lines.begin(), lines.end());
		change_line(lines_vec, line_result);
		draw_ladder(vec, line_result);
	}

	return 0;
}
