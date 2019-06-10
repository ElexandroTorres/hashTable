#include <iostream>

template<class KeyType, class DataType>
class HashEntry {
	public:
		HashEntry(KeyType k_, DataType d_) : m_key(k_), m_data(d_) {/* Empty */}

		KeyType m_key; // Amazena uma chave para uma entrada.
		DataType m_data; // Armazena o dado para uma entrada.

};

template<typename KeyType, 
		 typename DataType>,
		 typename KeyHash = std::hash<KeyType>
class Hashtbl {
	public:
		using Entry = HashEntry<KeyType, DataType>; //

		Hashtbl(size_t tbl_size_ = DEFAULT_SIZE) {

		}

		virtual ~Hashtbl() {

		}

		Hashtbl(const Hashtbl &other) {

		}

		Hashtbl(std::initializer_list<Entry> ilist) {

		}

		Hashtbl &operator=(const Hashtbl &other) {

		}

		Hashtbl &operator=(std::initializer_list<Entry> ilist) {

		}

		bool insert(const KeyType &k_, const DataType &d_) {

		}

		bool erase(const KeyType &k_) {

		}

		bool retrieve(const KeyType &k_, DataType &d_) const {

		}

		void clear(void) {

		}

		bool empty(void) const {

		}

		size_t size(void) const {

		}

		DataType &at(const KeyType &k_) {

		}

		DataType &operator[](const KeyType &k_) {

		}

		size_t count(const KeyType &k_) const {

		}

		friend std::ostream &operator<<(std::ostream &out, const Hashtbl &tbl) {

		}

	private:
		void rerash(); // muda o tamanho da tablea caso o fator seja maior que 1.
		unsigned int m_size;
		unsigned int m_count;

		std::forward_list<Entry> *m_data_table;


		static const short DEFAULT_SIZE = 11;
}