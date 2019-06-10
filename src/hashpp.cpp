#include <iostream>
#include "../include/list.h"
#include <iostream>
#include <unordered_map>
#include <forward_list>
#include <iterator>
#include <map>
struct Account {
	std::string nome;
	size_t codigo;
	size_t num_agencia;
	size_t nun_conta;
	size_t saldo;
};






template<class KeyType, class DataType>
class HashEntry {
	public:
		HashEntry(KeyType key_, DataType data_) : m_key(key_), m_data(data_) {/* Empty */}

		HashEntry() {}

		DataType &get_data() {
			return m_data;
		}

		KeyType &get_key() {
			return m_key;
		}

		KeyType m_key; // Amazena uma chave para uma entrada.
		DataType m_data; // Armazena o dado para uma entrada.

};



template<typename KeyType, typename DataType>
class Hashtbl {
	public:
		using Entry = HashEntry<KeyType, DataType>; //

		Hashtbl(size_t tbl_size_ = DEFAULT_SIZE) {
			m_data_table = new sc::list<Entry>[DEFAULT_SIZE]; // Cria uma tablea de listas encadeadas.
			m_size = tbl_size_;
			m_count = 0;
		}

		virtual ~Hashtbl() {
			delete[] m_data_table;
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
			std::hash<KeyType> hashFunc;
			std::equal_to<KeyType> equalFunc;
			Entry new_entry(k_, d_);

			auto end(hashFunc(k_) % m_size); //Representa o endereço.

			auto it = m_data_table[end].begin(); 

			//caso não tenham itens na lista.
			while(it != m_data_table[end].end()) {
				// Verificar se ja existe algum item com a mesma chave, caso sim não sera possivel completar a operação.
				if(new_entry.m_key == (*it).m_key) {
					return false;
				}
				it++;
			}

			m_data_table[end].push_back(new_entry);


			std::cout << "valor função hash: " << hashFunc(k_) << "\n";
			std::cout << "Valor da chave: " << end << "\n"; 
		}

		bool erase(const KeyType &k_) {
			
		}

		bool retrieve(const KeyType &k_, DataType &d_) const {

		}

		void clear(void) {

		}

		bool empty(void) const {
			return m_size == 0;
		}

		size_t size(void) const {
			return m_size;
		}

		DataType &at(const KeyType &k_) {

		}

		DataType &operator[](const KeyType &k_) {
			std::hash<KeyType> hashFunc;
			std::equal_to<KeyType> equalFunc;

			auto end(hashFunc(k_) % m_size);

			auto it = m_data_table[end].begin();

			while(it != m_data_table[end].end()) {
				//std::cout << "ENTRO AKI!!\n";
				if(k_ == (*it).m_key) {
					return (*it).m_data;
				}
				it++;
			}

		}

		size_t count(const KeyType &k_) const {

		}

		friend std::ostream &operator<<(std::ostream &out, const Hashtbl &tbl) {
			for(int i = 0; i < 10; i++) {
				auto it = tbl.m_data_table[i].begin();	
				out << i << ": ";
				while(it != tbl.m_data_table[i].end()) {
					out << (*it).m_data << " ";
					it++;
				}
				out << "\n";
			}
			
			return out;
		}

	private:
		void rerash(); // muda o tamanho da tablea caso o fator seja maior que 1.
		unsigned int m_size; //<! Tamanho da tabela.
		unsigned int m_count; //<! Quantidade de elementos na tabela.

		sc::list<Entry> *m_data_table; //<! Poneiro para tabela de listas de Entry.


		static const short DEFAULT_SIZE = 11;
};

int main() {


	//criar uma hashtable.
	Hashtbl<std::string, int> tabela(1);


	Hashtbl<char, int> htable(7);
    std::map<char, int> expected {{'x', 27}, {'y', 3}, {'w', 1}, {'a', 21}, {'b', 6}, {'c', 11}};

    if( htable.empty() ) {
    	std::cout << "PASSOU!!\n";
    }

    // Test insertion
    std::cout << "ORDEM DE INSERÇÃO: \n";
    for( const auto &e : expected )
    {
    	std::cout << e.first << "\n";
        auto result = htable.insert( e.first, e.second );
        if(result) {
        	std::cout << "PASSOU!!\n";
        }
    }

    std::cout << "Testes [] hash: \n";
    std::cout << "htable['c']: " << htable['c'] << "\n";


    std::cout << "Testes impressão tabela: \n";
    std::cout << htable << std::endl;




	return 0;

}