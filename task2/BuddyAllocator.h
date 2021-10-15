#include <math.h>
#include <unordered_set>
#include <map>
using namespace std;

class BuddyAllocator {
public:
	BuddyAllocator(int mem_size) {
		log_size = log2(mem_size);
		if (log_size != (int)log_size)
			throw "Ðàçìåð äèíàìè÷åñêîé ïàìÿòè íå 2^n!";
		memory = new char[1 << log_size];
		block_lists = new unordered_set<char*>[log_size + 1];
		(block_lists + log_size)->insert(memory);
		level_at_p[memory] = log_size;
	}

	char* allocate(int size) {
		char block_log_size = ceil(log2(size));
		char i = block_log_size; // íà ñëó÷àé, åñëè íàèìåíüøèé äîñòóïíûé áëîê áîëüøå ÷åì íóæíî
		char* block_start = nullptr; // óêàçàòåëü íà âûäåëåííûé áëîê ïàìÿòè (âîçâðàùàåìîå çíà÷åíèå)
		for (i; i <= log_size; i++) //èùåì íàèìåíüøèé äîñòóïíûé áëîê
			if ((block_lists + i)->size()) // ïðîâåðÿåì íà ïóñòîòó ìíîæåñòâî áëîêîâ î÷åðåäíîãî óðîâíÿ
				block_start = &(*(*(block_lists + i)->begin())); //åñëè íå ïóñòî, áåðåì ïåðâûé ïîïàâøèéñÿ áëîê
		if (!block_start) throw "Íåäîñòàòî÷íî ñâîáîäíîé ïàìÿòè!";
		for (i--; i > block_log_size; i--) { // ïîêà ìàêñèìàëüíî âîçìîæíî íå óìåíüøèëè äîñòóïíûé áëîê
			(block_lists + i)->erase(block_start); // äåëèì áëîê íà äâà, ñîõðàíÿÿ óêàçàòåëü íà ïåðâûé
			(block_lists + i - 1)->insert({ block_start, block_start + (1 << i - 1) });
			level_at_p[block_start + (1 << i - 1)] = i - 1;
		}
		(block_lists + i)->erase(block_start); // çàíèìàåì íàèìåíüøèé ïîëó÷åííûé áëîê
		level_at_p[block_start] = i;
		return block_start;
	}

	void free(char* p) {
		char level = level_at_p[p]; // ñìîòðèì, íà êàêîì óðîâíå áëîê ïî ýòîìó óêàçàòåëþ
		bool is_right_buddy = (p - memory) & (1 << level); // ÿâëÿåòñÿ ëè î÷åðåäíîé áëîê ïðàâûì áàääè
		char* buddy = is_right_buddy ? p - (1 << level) : p + (1 << level); // óêàçàòåëü íà áàääè
		auto cur_level_blocks = block_lists + level;
		while (level == level_at_p[buddy] // ïîêà ðÿäîì åñòü áàääè
			   && (block_lists + level)->find(buddy) != (block_lists + level)->end()) { // è áàääè ñâîáîäåí
			cur_level_blocks->erase(p); // óäàëÿåì áëîê è áàääè
			cur_level_blocks->erase(buddy);
			level_at_p.erase(is_right_buddy ? p : buddy);
			(cur_level_blocks + 1)->insert(p); // è äîáàâëÿåì íîâûé áëîê íà óðîâåíü âûøå
			level_at_p[is_right_buddy ? buddy : p]++;
			level++;
			is_right_buddy = (p - memory) & (1 << level); // îáíîâëÿåì ïåðåìåííûå âûøå
			buddy = is_right_buddy ? p - (1 << level) : p + (1 << level);
			cur_level_blocks = block_lists + level;
		}
	}

private:
	char log_size;
	char* memory;
	unordered_set<char*>* block_lists; // ìàññèâ ìíîæåñòâ óêàçàòåëåé íà áëîêè ñ îäèíàêîâûì ðàçìåðîì (ëîãàðèôìè÷åñêèì óðîâíåì)
	map<char*, char> level_at_p; // ñëîâàðü äëÿ áûñòðîãî ïîèñêà óðîâíÿ áëîêà ïî óêàçàòåëþ
};
