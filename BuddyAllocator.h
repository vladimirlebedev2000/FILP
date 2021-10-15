#include <math.h>
#include <unordered_set>
#include <map>
using namespace std;

class BuddyAllocator {
public:
	BuddyAllocator(int mem_size) {
		log_size = log2(mem_size);
		if (log_size != (int)log_size)
			throw "������ ������������ ������ �� 2^n!";
		memory = new char[1 << log_size];
		block_lists = new unordered_set<char*>[log_size + 1];
		(block_lists + log_size)->insert(memory);
		level_at_p[memory] = log_size;
	}

	char* allocate(int size) {
		char block_log_size = ceil(log2(size));
		char i = block_log_size; // �� ������, ���� ���������� ��������� ���� ������ ��� �����
		char* block_start = nullptr; // ��������� �� ���������� ���� ������ (������������ ��������)
		for (i; i <= log_size; i++) //���� ���������� ��������� ����
			if ((block_lists + i)->size()) // ��������� �� ������� ��������� ������ ���������� ������
				block_start = &(*(*(block_lists + i)->begin())); //���� �� �����, ����� ������ ���������� ����
		if (!block_start) throw "������������ ��������� ������!";
		for (i--; i > block_log_size; i--) { // ���� ����������� �������� �� ��������� ��������� ����
			(block_lists + i)->erase(block_start); // ����� ���� �� ���, �������� ��������� �� ������
			(block_lists + i - 1)->insert({ block_start, block_start + (1 << i - 1) });
			level_at_p[block_start + (1 << i - 1)] = i - 1;
		}
		(block_lists + i)->erase(block_start); // �������� ���������� ���������� ����
		level_at_p[block_start] = i;
		return block_start;
	}

	void free(char* p) {
		char level = level_at_p[p]; // �������, �� ����� ������ ���� �� ����� ���������
		bool is_right_buddy = (p - memory) & (1 << level); // �������� �� ��������� ���� ������ �����
		char* buddy = is_right_buddy ? p - (1 << level) : p + (1 << level); // ��������� �� �����
		auto cur_level_blocks = block_lists + level;
		while (level == level_at_p[buddy] // ���� ����� ���� �����
			   && (block_lists + level)->find(buddy) != (block_lists + level)->end()) { // � ����� ��������
			cur_level_blocks->erase(p); // ������� ���� � �����
			cur_level_blocks->erase(buddy);
			level_at_p.erase(is_right_buddy ? p : buddy);
			(cur_level_blocks + 1)->insert(p); // � ��������� ����� ���� �� ������� ����
			level_at_p[is_right_buddy ? buddy : p]++;
			level++;
			is_right_buddy = (p - memory) & (1 << level); // ��������� ���������� ����
			buddy = is_right_buddy ? p - (1 << level) : p + (1 << level);
			cur_level_blocks = block_lists + level;
		}
	}

private:
	char log_size;
	char* memory;
	unordered_set<char*>* block_lists; // ������ �������� ���������� �� ����� � ���������� �������� (��������������� �������)
	map<char*, char> level_at_p; // ������� ��� �������� ������ ������ ����� �� ���������
};