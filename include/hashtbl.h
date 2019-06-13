#ifndef HASHTBL_H
#define HASHTBL_H

#include <forward_list>
#include <cmath>

// Namespace
namespace ac {
	/// Classe HashEntry. Representa um item de tabela.
	template<class KeyType, class DataType>
	class HashEntry {
		public:
			KeyType m_key; //<! Amazena uma chave para uma entrada.
			DataType m_data; //<! Armazena o dado para uma entrada.
			/// Construtor padrão.
			HashEntry() {/* Vazio*/}
			/// Construtor com inicialização.
			HashEntry(KeyType key_, DataType data_) : m_key(key_), m_data(data_) {/* Empty */}

	};

	/// Classe HashTbl
	template<typename KeyType, typename DataType, typename KeyHash = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>>
	class HashTbl {
		public:
			// Definições de tipo:
			using Entry = HashEntry<KeyType, DataType>;
			// Construtores:

			/*!
			 * Construtor padrão. Caso não informado o tamanho a tabela será iniciada com o tamanho "DEFAULT_SIZE".
			*/
			HashTbl(size_t tbl_size_ = DEFAULT_SIZE) {
				m_tableSize = nextPrime(tbl_size_); // Define o tamanho da tabela como sendo o primo >= a tbl_size.
				m_data_table = new std::forward_list<Entry>[m_tableSize]; // Cria uma tabela de listas encadeadas.
			}
			/*!
			 * Construtor copia.
			*/
			HashTbl(const HashTbl &other) {
				m_tableSize = other.m_tableSize;
				m_count = other.m_count;
				m_data_table = new std::forward_list<Entry>[m_tableSize]; // Cria uma tabela de listas encadeadas.
				// Copiar todas as listas para a nova tabela.
				for(auto i = 0u; i < m_tableSize; i++) {
					m_data_table[i] = other.m_data_table[i];
				}
			}
			/*!
			 * Construtor a partir de outra lista.
			*/
			HashTbl(std::initializer_list<Entry> ilist) {
				m_tableSize = nextPrime(ilist.size());
				m_data_table = new std::forward_list<Entry>[m_tableSize]; // Cria uma tablea de listas encadeadas.
				// Inserir cada item da lista inicializadora na tabela.
				for(auto it = ilist.begin(); it != ilist.end(); it++) {
					insert(it->m_key, it->m_data);
				}
			}
			/*!
			 * Destrutor.
			*/
			virtual ~HashTbl() {
				delete[] m_data_table;
			}
			/*!
			 * Operador de atribuição a partir de outra tabela.
			*/
			HashTbl &operator=(const HashTbl &other) {
				//antes limpar a lista caso ela tenha elementos.
				if(this->m_count > 0) {
					clear();
				}
				m_tableSize = other.m_tableSize;
				m_count = other.m_count;
				delete[] m_data_table;
				m_data_table = new std::forward_list<Entry>[m_tableSize]; // Cria uma tablea de listas encadeadas.
				// Copiar todas as listas para a nova tabela.
				for(auto i = 0u; i < m_tableSize; i++) {
					m_data_table[i] = other.m_data_table[i];
				}

				return *this;
			}
			/*!
			 * Operador de atribuição a partir de uma lista inicializadora.
			*/
			HashTbl &operator=(std::initializer_list<Entry> ilist) {
				if(this->m_count > 0) {
					clear();
				}
				m_tableSize = nextPrime(ilist.size());
				delete[] m_data_table;
				m_data_table = new std::forward_list<Entry>[m_tableSize];
				//inserir os novos itens na lista.
				for(auto it = ilist.begin(); it != ilist.end(); it++) {
					insert(it->m_key, it->m_data);
				}

				return *this;

			}
			/*!
			 * Insere um novo elemento na tabela. Caso a chave ja esteja na tabela, subistituir o valor associado a ela.
			 * @param k_ Chave do novo elemento.
			 * @param d_ Dado associado a chave.
			 * @return true caso seja possivel inserir o novo elemento e false caso contrario.
			*/
			bool insert(const KeyType &k_, const DataType &d_) {
				
				Entry new_entry(k_, d_); // Cria um novo item de tabela com os valores passados.

				auto address(hashFunc(k_) % m_tableSize); // Calcula o endereço a qual o valor será adicionado.
				// Varrer toda a lista para verificar se há algum item com a mesma chave.
				for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
					// Caso encontre algum item com a mesma chave faz a substuição do dado associado a ela.
					if(equalFunc(new_entry.m_key, it->m_key)) {
						it->m_data = d_;		
						return false;
					}
				}
				// Verifica o fator para caso seja necessario de uma tabela com mais espaços.
				if((m_count/m_tableSize) > 1) {
					rerash();
					address = hashFunc(k_) % m_tableSize; // Calcula o endereço a qual o valor será adicionado.
				}
				// Caso seja a primeira vez de um item com essa chave, inserir ele na tabela.
				m_data_table[address].push_front(new_entry);

				++m_count;

				return true;

			}

			bool erase(const KeyType &k_) {
				auto address(hashFunc(k_) % m_tableSize); // Calcula o endereço a qual o valor será removido.
				
				auto it = m_data_table[address].begin();
				// Caso especial, lista vazia.
				if(it == m_data_table[address].end()) {
					return false;
				}
				// Caso especial, para caso o item removido seja o primeiro elemento.
				if(equalFunc(k_, it->m_key)) {
					m_data_table[address].erase_after(m_data_table[address].before_begin());
					--m_count; // Diminuí a quantidade de elementos presentes na tabela.
					return true;
				}
				auto itPrev = it; // Posição anterior ao it para nos permitir fazer a inserção.
				it++; // Avança o iterator.
				// Verificar o resto da lista.
				while(it != m_data_table[address].end()) {
					// Caso encontre um item com a mesma chave, iremos remove-lo.
					if(equalFunc(k_, it->m_key)) {
						m_data_table[address].erase_after(itPrev); // Remove o elemento após o itPrev, ou seja, o que achamos a igualdade entre as chaves.
						--m_count;					
						return true;
					}
					++itPrev; // ItPrev sempre vai está uma posição antes do it para assim permitir a inserção.
					++it;
				}
				return false;

			}
			/*!
			 * Dado uma chave verifica o elemento associado a ela e o atribui a uma variavel.
			 * @param k_ Chave para o elemento.
			 * @param d_ Referencia para onde será armazenado o valor associado a chave.
			 * @return true caso estejam associados e false caso contrario.
			*/
			bool retrieve(const KeyType &k_, DataType &d_) const {
				auto address(hashFunc(k_) % m_tableSize);

				for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
					if(equalFunc(k_, it->m_key)) {
						d_ = it->m_data;
						return true;
					}
				}

				return false;

			}
			/*!
			 * Limpa a tabela, liberando o espaço de todas as listas associadas a ela.
			*/
			void clear(void) {
				// Iremos limpar todas as listas. Porem a tabela ainda continuará com o mesmo tamanho.
				for(auto i = 0u; i < m_tableSize; i++) {
					m_data_table[i].clear();
				}
				m_count = 0;
			}
			/*!
			 * Verifica se a tabela está vazia.
			 * @return true Caso esteja vazia e false caso contrario.
			*/
			bool empty(void) const {
				return m_count == 0;
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

				auto address(hashFunc(k_) % m_tableSize); //Representa o endereço.
				unsigned int cont = 0; // Contador para verificar quantos elementos existem no mesmo "ramo" da chave dada.
				// Varrer toda a lista até o seu final.
				for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
					++cont;
				}
				return cont;

			}
			/*!
			 * Retorna uma referencia para o dado associado a chave dada. 
			 * Caso a chave não exista na tabela, retorna uma exceção.
			 * @param k_ Chave do elemento desejado.
			 * @return Uma referencia para o dado associado a chave dada ou uma exceção std::out_of_range caso não exista.
			*/
			DataType &at(const KeyType &k_) {
				auto address(hashFunc(k_) % m_tableSize);

				for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
					if(equalFunc(k_, it->m_key)) {
						return it->m_data;
					}
				}
				// Caso a chave não seja encontrada é lançada uma exceção.
				throw std::out_of_range("Key is not in table");
			}	
			/*!
			 * Retorna uma referencia para o dado associado a chave dada. 
			 * Caso a chave não exista ela será adicionada na tabela.
			 * @param k_ Chave do elemento desejado.
			 * @return Uma referencia para o dado associado a chave dada.
			*/
			DataType &operator[](const KeyType &k_) {
				auto address(hashFunc(k_) % m_tableSize);

				for(auto it = m_data_table[address].begin(); it != m_data_table[address].end(); it++) {
					if(equalFunc(k_, it->m_key)) {
						return it->m_data;
					}
				}
				//Caso a chave não esteja presente.
				insert(k_, DataType());

				return m_data_table[address].begin()->m_data;	

			}
			/*!
			 * Sobrecarga do operador << para permitir a impressão da tabela de maneira pratica.
			*/
			friend std::ostream &operator<<(std::ostream &out, const HashTbl &tbl) {
				for(auto i = 0u; i < tbl.m_tableSize; i++) {
					out << i << ": ";
					for(auto it = tbl.m_data_table[i].begin(); it != tbl.m_data_table[i].end(); it++) {
						out << it->m_data << " ";
					}
					out << std::endl;
				}
				return out;
			}

		private:
			KeyHash hashFunc; //<! Função hash.
			KeyEqual equalFunc; //<! Função de igualdade.
			unsigned int m_tableSize; //<! Armazena o tamanho da tabela..
			unsigned int m_count = 0; //<! Quantidade de elementos na tabela.

			std::forward_list<Entry> *m_data_table; //<! Poneiro para tabela de listas de Entry.

			static const short DEFAULT_SIZE = 11; // Tamanho padrão da tabela.

			// Metodos privados: 

			/*!
			 * Aumenta o tamanho da tabela e reorganiza os elementos da tabela anterior com novas posições de acordo com
			 * a função hash.
			*/
			void rerash() {
				unsigned int newTableSize = nextPrime(2*m_tableSize);
				std::forward_list<Entry> *newTable;
				newTable = new std::forward_list<Entry>[newTableSize];
				// Varrer toda a tabela anterior.
				for(auto i = 0u; i < m_tableSize; i++) {
					// Varrer cada posição da lista e adicionar os elementos na nova tabela em novas posições.
					for(auto it = m_data_table[i].begin(); it != m_data_table[i].end(); it++) {
						auto address(hashFunc(it->m_key) % newTableSize);
						newTable[address].push_front(*it);
					}
				}

				delete[] m_data_table; // Apaga a memoria associada a tabela anterior.

				m_data_table = newTable;
				m_tableSize = newTableSize;
			}
			/*!
			 * Verifica se um determinado numero é primo.
			 * @param number Numero a ser verificado.
			 * @return true caso seja primo e false caso contrario.
			*/
			bool is_prime(size_t &number) {
				for(size_t i = 2; i <= sqrt(number); i++) {
					if(number % i == 0) {
						return false;
					}
				}
				return true;
			}
			/*!
			 * Dado um numero calcular o primo que é igual ou o segue.
			 * @param number Numero a qual iremos calcular.
			 * @return O numero primo igual ou seguinte ao numero dado.
			*/
			size_t nextPrime(size_t number) {
				if(number <= 1) {
					return 1;
				}
				// Enquanto não achar um primo vai testando para o proximo valor.
				while(is_prime(number) == false) {
					number++;
				}
				return number;
			}
	}; // Fim da Classe hashTbl
} // Fim do Namespace

#endif
