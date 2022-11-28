#include <memory>
#include <unordered_map>
#include <unordered_set>
using namespace std;

constexpr int max_depth = 5;

template<typename key, typename value_t>
struct fragment{
	shared_ptr<fragment> parent;
	unordered_map<key, value_t> u_map;
	unordered_map<key, value_t> m_map;
	unordered_set<key> deleted_keys;
	fragment absolute_fragment() {
		fragment ret;
		if (parent) ret = parent->absolute_fragment();
		for (auto& it : deleted_keys) ret.u_map.erase(it);
		for (auto& it : u_map) ret.u_map.insert(it);
		for (auto& it : m_map) {
			ret.u_map[it.first] = it.second;
		}
		return ret;
	}
 	value_t& find(key k) {
		if (u_map.find(k) != u_map.end()) {
			return u_map[k];
		}
		if (m_map.find(k) != m_map.end()) {
			return m_map[k];
		}
		if (parent)
		  m_map[k] = parent->find(k);
		return m_map[k];
	}
	void update_values() {
		for (auto it:m_map) {
			u_map[it.first] = it.second;
		}
		m_map.clear();
	}
	~fragment() {
		cout<<"gone\n";
	}
};

template<typename key, typename value_t>
struct shared_map{
  typedef fragment<key, value_t> frag;
	shared_map() : depth(1) {
		head = make_shared<frag>();
	}
	void erase(key k) {
		if (head.use_count() > 1) this->new_head();
		head->u_map.erase(k);
		head->deleted_keys.insert(k);
	}
	void insert(key k, value_t value) {
		if (head.use_count() > 1) this->new_head();
		head->u_map[k] = value;
		head->m_map[k] = value;
	}
	value_t& operator[](key k) {
		if (head.use_count() > 1) this->new_head();
		return head->find(k);
	}
	void new_head() {
		if (depth < max_depth) {
			auto temp = head;
			head = make_shared<frag>();
			head->parent = temp;
			depth++;
		}
		else  {
			head = make_shared<frag>(head->absolute_fragment());
		}
	}
	void finish_iteration() {
		head->update_values();
	}
	int depth;
	shared_ptr<frag> head;
};