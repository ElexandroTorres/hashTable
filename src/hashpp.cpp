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



template<typename KeyType, typename DataType, typename KeyHash = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>>
class Hashtbl {
	public:
		// Definições de tipo:
		using Entry = HashEntry<KeyType, DataType>;
		// Construtores:

		/*!
		 * Construtor padrão. Caso não informado o tamanho a tabela será iniciada com o tamanho "DEFAULT_SIZE".
		*/
		Hashtbl(size_t tbl_size_ = DEFAULT_SIZE) {
			m_Tablesize = tbl_size_;
			m_count = 0; // Quantidade inicial de elementos é zero.
			m_data_table = new std::forward_list<Entry>[m_Tablesize]; // Cria uma tablea de listas encadeadas.
		}
		/*!
		 * Construtor copia.
		*/
		Hashtbl(const Hashtbl &other) {

		}
		/*!
		 * Construtor a partir de outra lista.
		*/
		Hashtbl(std::initializer_list<Entry> ilist) {

		}
		/*!
		 * Destrutor.
		*/
		virtual ~Hashtbl() {
			delete[] m_data_table;
		}

		Hashtbl &operator=(const Hashtbl &other) {

		}

		Hashtbl &operator=(std::initializer_list<Entry> ilist) {

		}
		/*!
		 * Insere um novo elemento na tabela.
		 * @param k_ Chave do novo elemento.
		 * @param d_ Dado associado a chave.
		 * @return true caso seja possivel inserir o novo elemento e false caso contrario.
		*/
		bool insert(const KeyType &k_, const DataType &d_) {
			
			Entry new_entry(k_, d_); // Cria um novo item de tabela com os valores passados.

			auto address(hashFunc(k_) % m_Tablesize); // Calcula o endereço a qual o valor será adicionado.
			// Varrer toda a lista para verificar se há algum item com a mesma chave.
			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				// Caso encontre algum item com a mesma chave não será possivel fazer a inserção.
				if(new_entry.m_key == it->m_key) {
					return false;
				}
			}
			// Caso seja a primeira vez de um item com essa chave, inserir ele na tabela.
			m_data_table[address].push_front(new_entry);

			return true;

		}

		bool erase(const KeyType &k_) {
			
			auto address(hashFunc(k_) % m_Tablesize); // Calcula o endereço a qual o valor será removido.
			// Varrer toda a lista para verificar se há algum item com a mesma chave.
			auto it = m_data_table[address].begin();
			auto itPrev = it; // Posição anterior ao it para nos permitir fazer a inserção.
			// Caso especial, para caso o item removido seja o primeiro elemento.
			if(k_ == it->m_key) {
				m_data_table[address].erase_after(m_data_table[address].before_begin());
				--m_count; // Diminuí a quantidade de elementos presentes na tabela.
				return true;
			}
			it++; // Avança o iterator.
			// Verificar o resto da lista.
			while(it != m_data_table[address].end()) {
				// Caso encontre um item com a mesma chave, iremos remove-lo.
				if(k_ == it->m_key) {
					m_data_table[address].erase_after(itPrev); // Remove o elemento após o itPrev, ou seja, o que achamos a igualdade entre as chaves.
					--m_count;					
					return true;
				}
				it++;
				itPrev++; // ItPrev sempre vai está uma posição antes do it para assim permitir a inserção.
			}

			return false;

		}
		/*!
		 * Verifica se uma chave e um dado estão associados na tabela.
		 * @param k_ Chave para o elemento.
		 * @param d_ Dado do elemento.
		 * @return true caso estejam associados e false caso contrario.
		*/
		bool retrieve(const KeyType &k_, DataType &d_) const {
			auto address(hashFunck(k_) % m_Tablesize);

			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				if(k_ == it->m_key) {
					if(d_ == it->m_data) {
						return true;
					}
					return false;
				}
			}

			return false;

		}

		void clear(void) {

		}
		/*!
		 * Verifica se a tabela está vazia.
		 * @return true Caso esteja vazia e false caso contrario.
		*/
		bool empty(void) const {
			return m_Tablesize == 0;
		}
		/*!
		 * Retorna a quantidade de elementos presentes na tabela.
		 * @return Quantidade de elementos na tabela.
		*/
		size_t size(void) const {
			return m_count;
		}
		/*!
		 * Retorna a quantidade de elementos presentes na lista de colisão do elemento da chave dada.
		 * @param k Chave para um determinado elemento na tabela.
		 * @return Quantidade de elementos presentes na lista de colisão em que k_ pertence.
		*/
		size_t count(const KeyType &k_) const {

			auto address(hashFunc(k_) % m_Tablesize); //Representa o endereço.
			unsigned int cont = 0; // Contador para verificar quantos elementos existem no mesmo "ramo" da chave dada.
			// Varrer toda a lista até o seu final.
			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				++cont;
			}
			return cont;

		}

		DataType &at(const KeyType &k_) {
			auto address(hashFunc(k_) % m_Tablesize);

			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				if(k_ == it->m_key) {
					return it->m_data;
				}
			}
			// Caso a chave não seja encontrada é lançada uma exceção.
			throw std::out_of_range("Key is not in table");
		}

		DataType &operator[](const KeyType &k_) {
			auto address(hashFunc(k_) % m_Tablesize);

			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				if(k_ == it->m_key) {
					return it->m_data;
				}
			}

		}
		/* Caso seja necessario.
		const DataType &operator[](const KeyType &k_) const {
			auto address(hashFunc(k_) % m_Tablesize);

			for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
				if(k_ == it->m_key) {
					return it->m_data;
				}
			}
		}
		*/
		/*!
		 * Sobrecarga do operador << para permitir a impressão da tabela de maneira pratica.
		*/
		friend std::ostream &operator<<(std::ostream &out, const Hashtbl &tbl) {
			for(auto i = 0u; i < tbl.m_Tablesize; i++) {
				out << i << ": ";
				for(auto it = tbl.m_data_table[i].begin(); it != tbl.m_data_table[i].end(); it++) {
					out << it->m_data << " ";
				}
				out << std::endl;
			}
			return out;
		}

	private:
		KeyHash hashFunc;
		KeyEqual equalFunc;
		void rerash(); // muda o tamanho da tablea caso o fator seja maior que 1.
		unsigned int m_Tablesize; //<! Armazena o tamanho da tabela..
		unsigned int m_count; //<! Quantidade de elementos na tabela.

		std::forward_list<Entry> *m_data_table; //<! Poneiro para tabela de listas de Entry.


		static const short DEFAULT_SIZE = 11; // Tamanho padrão da tabela.
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

    std::cout << "Testes count: \n";
    std::cout << htable.count('b') << "\n";

    htable['c'] = 24;
    //htable.erase('a');
    htable.erase('c');
    //std::cout << "htable['c']: " << htable['c'] << "\n";

    std::cout << "Testes impressão tabela: \n";
    std::cout << htable << std::endl;




	return 0;

}