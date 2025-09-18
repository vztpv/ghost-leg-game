
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <thread>

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

void change_line(int len, std::vector<Line>& lines, std::vector<Line>& result) {
	if (lines.empty()) {
		return;
	}
	
	int max = lines[0].level;
	for (uint64_t i = 0; i < lines.size(); ++i) {
		if (max < lines[i].level) {
			max = lines[i].level;
		}
	}
	for (uint64_t i = 0; i < lines.size(); ++i) {
		lines[i].level = max - lines[i].level; // level starts from 0
	}

	std::reverse(lines.begin(), lines.end());

	std::vector<int> temp(len, -1); // vec.size()

	result.push_back(lines[0]);
	result.back().level = 0;

	temp[lines[0].left] = 0; 
	temp[lines[0].right] = 0;

	int level = 0;
	
	for (uint64_t i = 1; i < lines.size(); ++i) {
		if (lines[i - 1].level == lines[i].level && lines[i - 1].right == lines[i].left) {
			++level;

			Line line{ level, lines[i].left, lines[i].right };
			
			if (temp[lines[i].left] > -1 && temp[lines[i].left] + 1 < level
				&& temp[lines[i].right] > -1 && temp[lines[i].right] + 1 < level) {
				line.level = std::max(temp[lines[i].left], temp[lines[i].right]) + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else if (temp[lines[i].left] > -1 && temp[lines[i].left] + 1 < level
				&& temp[lines[i].right] == -1 // && temp[lines[i].right] + 1 < level
				) {
				line.level = temp[lines[i].left] + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else if (temp[lines[i].left] == -1 && //temp[lines[i].left] + 1 < level
				temp[lines[i].right] > -1 && temp[lines[i].right] + 1 < level
				) {
				line.level = temp[lines[i].right] + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else {
				temp[lines[i].left] = level;
				temp[lines[i].right] = level;
			}

			result.push_back(line);
		}
		else {
			bool flag = false;

			// lines[i].level - lines[i - 1].level == 0 or 1
			if (lines[i - 1].level == lines[i].level && lines[i - 1].right >= lines[i].right) {
				if (level == 0) {
					level = 1;
				}
				else {
					++level;
					flag = true;
				}
			}
			else if (lines[i - 1].level < lines[i].level) {
				if (level == 0) {
					level = 1;
				}
				else {
					++level;
					flag = true;
				}
			}
			else {
				if (level == 0) {
					level = 1;
				}
			}

			Line line{ level, lines[i].left, lines[i].right };

			if (temp[lines[i].left] > -1 && temp[lines[i].left] + 1 < level
				&& temp[lines[i].right] > -1 && temp[lines[i].right] + 1 < level) {
				line.level = std::max(temp[lines[i].left], temp[lines[i].right]) + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else if (temp[lines[i].left] > -1 && temp[lines[i].left] + 1 < level
				&& temp[lines[i].right] == -1 //&& temp[lines[i].right] + 1 < level
				) {
				line.level = temp[lines[i].left] + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else if (temp[lines[i].left] == -1 && // temp[lines[i].left] + 1 < level
				temp[lines[i].right] > -1 && temp[lines[i].right] + 1 < level
				) {
				line.level = temp[lines[i].right] + 1;
				temp[lines[i].left] = line.level;
				temp[lines[i].right] = line.level;
			}
			else {
				temp[lines[i].left] = level; 
				temp[lines[i].right] = level;
				level += lines[i].level - lines[i - 1].level;
			}

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
	uint64_t max_level = now_level;

	for (uint64_t i = 0; i < line_result.size(); ++i) {
		if (max_level < line_result[i].level) {
			max_level = line_result[i].level;
		}
	}

	std::vector<std::set<Line>> table(max_level - min_level + 1);

	for (uint64_t i = 0; i < line_result.size(); ++i) {
		table[line_result[i].level - min_level].insert(line_result[i]);
	}

	for (uint64_t i = 0; i < table.size(); ++i) {
		int k = 0;	
		auto iter = table[i].begin();
		for (; k < ladder_size; ++k) {
			if (iter != table[i].end() && k < iter->left) {
				std::cout << "|\t";
			}
			else if (iter != table[i].end() && k == iter->left) {
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


#include <Windows.h>

////
// 콘솔 색상 상수
enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clear_screen() {
    system("cls");
}

void draw_static_ladder(const std::vector<int>& result, const std::vector<Line>& line_result,
    uint64_t ladder_size, const std::vector<std::set<Line>>& table) {
    // 상단 인덱스 출력 (흰색)
    set_color(WHITE);
    gotoxy(0, 0);
    for (uint64_t i = 0; i < ladder_size; ++i) {
        gotoxy(i * 8, 0);
        std::cout << i;
    }

    // 각 레벨별로 라인 그리기 (회색)
    set_color(GRAY);
    for (uint64_t level = 0; level < table.size(); ++level) {
        int row = level + 1;

        // 세로줄 그리기
        for (uint64_t col = 0; col < ladder_size; ++col) {
            gotoxy(col * 8, row);
            std::cout << "|";
        }

        // 가로줄 그리기
        auto iter = table[level].begin();
        for (; iter != table[level].end(); ++iter) {
            int left_pos = iter->left * 8;
            int right_pos = iter->right * 8;

            gotoxy(left_pos + 1, row);
            for (int i = left_pos + 1; i < right_pos; ++i) {
                std::cout << "-";
            }
        }
    }

    // 하단 세로줄
    int result_row = table.size() + 1;
    for (uint64_t i = 0; i < ladder_size; ++i) {
        gotoxy(i * 8, result_row);
        std::cout << "|";
    }

    // 하단 결과 출력 (흰색)
   // set_color(WHITE);
   // for (uint64_t i = 0; i < ladder_size; ++i) {
    //    gotoxy(i * 8, result_row + 1);
    //    std::cout << result[i];
   // }
}

// 특정 위치의 숫자를 강조 표시
void highlight_number(int pos, int row, int number, int color) {
    gotoxy(pos * 8, row);
    set_color(color);
    std::cout << number;
}

// 가로줄을 강조 표시하는 함수
void highlight_horizontal_line(int left_pos, int right_pos, int row, int color) {
    gotoxy(left_pos * 8 + 1, row);
    set_color(color);
    for (int i = left_pos * 8 + 1; i < right_pos * 8; ++i) {
        std::cout << "-";
    }
}

// 가로줄을 원래 색으로 복원하는 함수
void restore_horizontal_line(int left_pos, int right_pos, int row) {
    gotoxy(left_pos * 8 + 1, row);
    set_color(GRAY);
    for (int i = left_pos * 8 + 1; i < right_pos * 8; ++i) {
        std::cout << "-";
    }
}

// 사다리 경로를 계산하는 함수 (가로줄 정보 포함)
struct PathStep {
    int pos;
    int row;
    bool has_horizontal_line;
    int line_left;
    int line_right;
};

std::vector<PathStep> calculate_path(int start_pos, const std::vector<std::set<Line>>& table) {
    std::vector<PathStep> path;
    int current_pos = start_pos;

    // 시작 위치
    path.push_back({ current_pos, 0, false, -1, -1 });

    // 각 레벨을 순회하며 경로 계산
    for (uint64_t level = 0; level < table.size(); ++level) {
        int row = level + 1;

        // 현재 레벨에서 연결된 가로줄 찾기
        bool found_connection = false;
        int line_left = -1, line_right = -1;

        for (const Line& line : table[level]) {
            if (line.left == current_pos) {
                // 오른쪽으로 이동
                current_pos = line.right;
                found_connection = true;
                line_left = line.left;
                line_right = line.right;
                break;
            }
            else if (line.right == current_pos) {
                // 왼쪽으로 이동
                current_pos = line.left;
                found_connection = true;
                line_left = line.left;
                line_right = line.right;
                break;
            }
        }

        path.push_back({ current_pos, row, found_connection, line_left, line_right });
    }

    // 마지막 결과 행 추가
    path.push_back({ current_pos, static_cast<int>(table.size() + 1), false, -1, -1 });

    return path;
}

void animate_ladder_descent(const std::vector<int>& result, const std::vector<Line>& line_result) {
    uint64_t ladder_size = result.size();

    if (line_result.empty()) {
        std::cout << "No lines to animate!" << std::endl;
        return;
    }

    // 레벨 범위 계산
    uint64_t min_level = line_result[0].level;
    uint64_t max_level = line_result[0].level;

    for (const Line& line : line_result) {
        if (min_level > line.level) min_level = line.level;
        if (max_level < line.level) max_level = line.level;
    }

    // 레벨별로 라인 그룹화
    std::vector<std::set<Line>> table(max_level - min_level + 1);
    for (const Line& line : line_result) {
        table[line.level - min_level].insert(line);
    }

    clear_screen();
    draw_static_ladder(result, line_result, ladder_size, table);

    std::cout << "\n\nPress enter key to start animation...";
    getchar();

    // 각 시작 위치별로 애니메이션
    std::vector<int> colors = { LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_YELLOW,
                              LIGHT_CYAN, LIGHT_MAGENTA, WHITE };

    for (uint64_t start = 0; start < ladder_size; ++start) {
        clear_screen();
        draw_static_ladder(result, line_result, ladder_size, table);

        // 경로 계산
        std::vector<PathStep> path = calculate_path(start, table);

        gotoxy(0, table.size() + 3);
        set_color(WHITE);
        std::cout << "Position " << start << " descending...";

        // 애니메이션 실행
        for (size_t i = 0; i < path.size(); ++i) {
            int pos = path[i].pos;
            int row = path[i].row;

            if (row == 0) {
                // 상단 인덱스 강조
                highlight_number(pos, row, start, colors[start]);
            }
            else if (row == static_cast<int>(table.size() + 1)) {
                // 하단 결과 강조
                highlight_number(pos, row, result[pos], colors[start]);
            }
            else {
                // 현재 위치에 숫자 표시
                highlight_number(pos, row, start, colors[start]);

                // 가로줄이 있다면 강조 표시
                if (path[i].has_horizontal_line) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    highlight_horizontal_line(path[i].line_left, path[i].line_right, row, colors[start]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));

                    // 가로줄 원래 색으로 복원
                    restore_horizontal_line(path[i].line_left, path[i].line_right, row);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));

                // 원래 세로줄로 복원
                gotoxy(pos * 8, row);
                set_color(GRAY);
                std::cout << "|";
            }

            if (i < path.size() - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // 최종 결과 모두 강조
    gotoxy(0, table.size() + 3);
    set_color(WHITE);
    std::cout << "Final result highlighted!        ";

    for (uint64_t i = 0; i < ladder_size; ++i) {
        highlight_number(i, table.size() + 1, result[i], colors[i]);
    }

    set_color(WHITE);
    gotoxy(0, table.size() + 5);
    std::cout << "Animation complete! Press any key to exit...";
}

void draw_ladder2(const std::vector<int>& result, const std::vector<Line>& line_result) {
    animate_ladder_descent(result, line_result);
}
////////////

int main(void)
{
	std::random_device rd;
	std::mt19937 g(rd());
	//srand(time(nullptr));
	for (int i = 0; i < 1; ++i) {
		std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6 }; // start from 0
		std::set<Line> lines;


		{

			std::shuffle(vec.begin(), vec.end(), g);
			//std::reverse(vec.begin(), vec.end());
			//std::random_shuffle(vec.begin(), vec.end());
			//vec = { 5, 3, 6, 2, 4, 1, 0 };
			//vec = { 3, 0 ,6, 4, 2, 5, 1 };
			// 2, 4, 0, 6, 1, 
		}

		std::cout << "\n";
		{
			std::vector<int> vec2 = vec;
			bubble_sort(vec2, lines);
			std::vector<Line> line_result;
			std::vector<Line> lines_vec(lines.begin(), lines.end());
			change_line(vec.size(), lines_vec, line_result);
			// draw_ladder <- no animation effect
			// draw_ladder2 <- animation effect!
			draw_ladder2(vec, line_result);
		}
	}
	return 0;
}
