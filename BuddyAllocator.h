#include <math.h>
#include <unordered_set>
#include <map>
using namespace std;

class BuddyAllocator {
public:
	BuddyAllocator(int mem_size) {
		log_size = log2(mem_size);
		if (log_size != (int)log_size)
			throw "Размер динамической памяти не 2^n!";
		memory = new char[1 << log_size];
		block_lists = new unordered_set<char*>[log_size + 1];
		(block_lists + log_size)->insert(memory);
		level_at_p[memory] = log_size;
	}

	char* allocate(int size) {
		char block_log_size = ceil(log2(size));
		char i = block_log_size; // на случай, если наименьший доступный блок больше чем нужно
		char* block_start = nullptr; // указатель на выделенный блок памяти (возвращаемое значение)
		for (i; i <= log_size; i++) //ищем наименьший доступный блок
			if ((block_lists + i)->size()) // проверяем на пустоту множество блоков очередного уровня
				block_start = &(*(*(block_lists + i)->begin())); //если не пусто, берем первый попавшийся блок
		if (!block_start) throw "Недостаточно свободной памяти!";
		for (i--; i > block_log_size; i--) { // пока максимально возможно не уменьшили доступный блок
			(block_lists + i)->erase(block_start); // делим блок на два, сохраняя указатель на первый
			(block_lists + i - 1)->insert({ block_start, block_start + (1 << i - 1) });
			level_at_p[block_start + (1 << i - 1)] = i - 1;
		}
		(block_lists + i)->erase(block_start); // занимаем наименьший полученный блок
		level_at_p[block_start] = i;
		return block_start;
	}

	void free(char* p) {
		char level = level_at_p[p]; // смотрим, на каком уровне блок по этому указателю
		bool is_right_buddy = (p - memory) & (1 << level); // является ли очередной блок правым бадди
		char* buddy = is_right_buddy ? p - (1 << level) : p + (1 << level); // указатель на бадди
		auto cur_level_blocks = block_lists + level;
		while (level == level_at_p[buddy] // пока рядом есть бадди
			   && (block_lists + level)->find(buddy) != (block_lists + level)->end()) { // и бадди свободен
			cur_level_blocks->erase(p); // удаляем блок и бадди
			cur_level_blocks->erase(buddy);
			level_at_p.erase(is_right_buddy ? p : buddy);
			(cur_level_blocks + 1)->insert(p); // и добавляем новый блок на уровень выше
			level_at_p[is_right_buddy ? buddy : p]++;
			level++;
			is_right_buddy = (p - memory) & (1 << level); // обновляем переменные выше
			buddy = is_right_buddy ? p - (1 << level) : p + (1 << level);
			cur_level_blocks = block_lists + level;
		}
	}

private:
	char log_size;
	char* memory;
	unordered_set<char*>* block_lists; // массив множеств указателей на блоки с одинаковым размером (логарифмическим уровнем)
	map<char*, char> level_at_p; // словарь для быстрого поиска уровня блока по указателю
};