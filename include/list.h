#ifndef LIST_H
#define LIST_H

/// Namespace sc
namespace sc {
	/// Classe list.
	template<typename T>
	class list {
		private:
			struct Node {
				T data; //<! Dados do elemento.
				Node *prev; //<! Nó que aponta para o Nó anterior.
				Node *next; //<! Nó que aponta para o Nó seguinte.
				/// Construtor basico do nó.
				Node(const T &data_ = T(), Node *prev_ = nullptr, Node *next_ = nullptr) {
					data = data_;
					prev = prev_;
					next = next_;
				}
			};

			/*! Cria os nós principais da lista duplamente encadeada. Nó cabeça e Nó Calda. */
			void create_mainNodes() {
				m_head = new Node(); // Novo nó cabeça;
				m_tail = new Node(); // Novo nó calda.
				m_head->next = m_tail; // Liga o nó cabeça ao nó calda.
				m_tail->prev = m_head; // Liga o nó calda ao nó cabeça.
			}



		public:
			// Definições.
			using size_type = unsigned long;
			/* Iterators */
			/// Const_Iterator
			class const_iterator {
				public:
					// Definições de tipo.
					using reference = T&;
					using difference_type = std::ptrdiff_t;
					using iterator_category = std::bidirectional_iterator_tag;
					using value_type = T; //!< Tipo de dado.
					using pointer = T*; //!< Ponteiro para o tipo de dado.
					/*! Construtor padrão. Cria um iterator e inicializa o current como nulo. */
					const_iterator() : current{nullptr} {/* Vazio.*/}
					/*! Construtor copia. */
					const_iterator(const const_iterator &other) {
						current = other.current;
					}
					/*! Operador*, returna o dado presente no iterator. */
					const T & operator*() const {
						return current->data;
					}
					/*! Operator++, avança para o proximo nó. */
					const_iterator &operator++() {
						current = current->next;
						return *this;
					}
					/*! Operator++, Avança para o proximo nó, mas retorna a posição antes de avançada. */
					const_iterator operator++(int) {
						const_iterator temp = *this;
						++(*this);
						return temp;
					}
					/*! Operator--, Regride para o nó anterior. */
					const_iterator &operator--() {
						current = current->prev;
						return *this;
					}
					/*! Operator--, Regride para o nó anterior, mas retorna a posição antes da regreção. */
					const_iterator operator--(int) {
						const_iterator temp = *this;
						--(*this);
						return temp;
					}
					/*! Operator =, Atribuí um const_iterator a outro.*/
					const_iterator &operator=(const const_iterator &it) {
						current = it.current;
						return *this;
					}
					/*! Compara dois iteradores constantes. 
					 * @return true caso sejam iguais e false caso contrario. 
					*/
					bool operator==(const const_iterator &rhs) const {
						return current == rhs.current;
					}
					/*! Compara dois iteradores constantes. 
					 * @return true caso sejam diferentes e false caso contrario. 
					*/
					bool operator!=(const const_iterator &rhs) const {
						return !(*this == rhs);
					}

				protected:
					Node *current;
					const_iterator(Node *p) : current{p} {/*vazio.*/}
					friend class list<T>;

			};
			/// Iterator
			class iterator {
				public:
					// Definições de tipo.
					using reference = T&;
					using difference_type = std::ptrdiff_t;
					using iterator_category = std::bidirectional_iterator_tag;
					using value_type = T; //!< Tipo de dado.
					using pointer = T*; //!< Ponteiro para o tipo de dado.
					/*! Construtor padrão. */
					iterator() : current{nullptr} {/* Vazio */}
					/*! Construtor copia. */
					iterator(const iterator &other) {
						current = other.current;
					}
					/*! Retorna uma referencia constante para o dado do iterator. */
					const T &operator*() const {
						return current->data;
					}
					/*! Retorna uma referencia para o dado do iterator, assim possibilitando a sua alteração. */
					T &operator*() {
						return current->data;
					}
					/*! Operator++, avança para o proximo nó. */
					iterator &operator++() {
						current = current->next;
						return *this;
					}
					/*! Operator++, Avança para o proximo nó, mas retorna a posição antes de avançada. */
					iterator operator++(int) {
						iterator temp = *this;
						++(*this);
						return temp;
					}
					/*! Operator--, Regride para o nó anterior. */
					iterator &operator--() {
						current = current->prev;
						return *this;
					}
					/*! Operator--, Regride para o nó anterior, mas retorna a posição antes da regreção. */
					iterator operator--(int) {
						iterator temp = *this;
						--(*this);
						return temp;
					}
					/*! Operator =, Atribuí um iterator a outro.*/
					iterator &operator=(const iterator &it) {
						current = it.current;
						return *this;
					}
					/*! Operator =, atribuí um const_iterator a um iterator. */ 
					iterator &operator=(const const_iterator &it) {
						current = it.current;
						return *this;
					}
					/*! Compara dois iteradores. 
					 * @return true caso sejam iguais e false caso contrario. 
					*/
					bool operator==(const iterator &rhs) const {
						return current == rhs.current;
					}
					/*! Compara dois iteradores constantes. 
					 * @return true caso sejam diferentes e false caso contrario. 
					*/
					bool operator!=(const iterator &rhs) const {
						return !(*this == rhs);
					}
					
					
				protected:
					Node *current;
					iterator(Node *p) : current{p} {/*vazio.*/}
					friend class list<T>;

			};

			/// Construtor padrão.
			list() {
				create_mainNodes(); // Cria os nós cabeça e calda da lista.
			}
			/// Construtor explicit.
			explicit list(size_type count) {
				create_mainNodes();
				for(size_type i = 0; i < count; i++) {
					push_back(int()); // Cria um nó e o inicia com o tipo inteiro padrão.
				}
			}
			/// Construtor copia.
			list(const list &other) {
				create_mainNodes();
				*this = other;
			}
			/// Construtor a partir de um range.
			template<typename InputIt>
			list(InputIt first, InputIt last) {
				create_mainNodes();
				while(first != last) {
					push_back(*first);
					first++;
				}
			}
			/// Construtor com lista inicializadora.
			list(std::initializer_list<T> ilist) {
				create_mainNodes();
				auto ilistTemp = ilist.begin();
				// Adicionar cada item da lista inicializadora no final da lista.
				while(ilistTemp != ilist.end()) { 
					push_back(*ilistTemp);
					ilistTemp++;
				}
			}
			/// Destrutor.
			~list() {
				// Remover todos os nós.
				clear();
				// Por fim remover o head e o tail.
				delete m_head;
				delete m_tail;
			}
			/*! Operador de atribuição a partir de outra lista. */
			list &operator=(const list &other) {
				// Verifica se são diferentes.
				if(*this != other) {
					clear(); // Limpa a lista.
					Node *temp1 = other.m_head;
					temp1 = temp1->next;
					// Varre toda a lista other adicionando todos os elementos na lista atual.
					while(temp1 != other.m_tail) {
						push_back(temp1->data);
						temp1 = temp1->next;
					}
				}
				return *this;
			}
			/*! Operador de atribuição a partir de uma lista inicializadora. */
			list &operator=(std::initializer_list<T> ilist) {
				clear();
				auto ilistTemp = ilist.begin();
				while(ilistTemp != ilist.end()) {
					push_back(*ilistTemp);
					ilistTemp++;
				}
				return *this;
			}

			/* Operations */

			/*!
			 * @return O numero de elementos da lista.
			*/
			size_type size() const {
				return m_size;
			}
			/*!
			 * Limpa a lista, excluindo os elementos e liberando a memoria dos nós que os armazena.
			*/
			void clear() {
				while(m_head->next != m_tail) {
					pop_back();
				}
				m_size = 0;
			}
			/*!
			 * Verifica se a lista está vazia.
			 * @return true caso esteja vazia e false caso contrario.
			*/
			bool empty() {
				return (m_size == 0);
			}
			/*!
			 * Adiciona um determinado valor no inicio da lista.
			 * @param value Valor a ser adicionado no inicio da lista.
			*/
			void push_front(const T &value) {
				Node *temp = m_head->next;
				Node *newNode = new Node(value, m_head, temp);
				m_head->next = newNode;
				temp->prev = newNode;
				m_size++;
			}
			/*!
			 * Adiciona um determinado valor no final da lista.
			 * @param value Valor a ser adicionado no final da lista.
			*/
			void push_back(const T &value) {
				Node *temp = m_tail->prev;
				Node *newNode = new Node(value, temp, m_tail);
				m_tail->prev = newNode;
				temp->next = newNode;
				m_size++;
			}
			/*!
			 * Remove o elemento no final da lista.
			*/
			void pop_back() {
				if(m_tail->prev != m_head) {
					Node *temp = m_tail->prev;
					Node *target = m_tail->prev;
					temp = temp->prev;
					delete target;
					temp->next = m_tail;
					m_tail->prev = temp;
					m_size--;
				}
			}
			/*!
			 * Remove o elemento no inicio da lista.
			*/
			void pop_front() {
				if(m_head->next != m_tail) {
					Node *temp = m_head->next;
					Node *target = m_head->next;
					temp = temp->next;
					delete target;
					temp->prev = m_head;
					m_head->next = temp;
					m_size--;
				}
			}
			/*!
			 * @return O elemento no final da lista. Caso a lista esteja vazia, retorna um std::range_error.
			*/
			const T &back() const {
				if(m_tail->prev != m_head) {
					Node *target = m_tail->prev;
					return target->data;
				}
				// Caso a lista esteja vazia.
				else {
					throw std::range_error("List is empty!");//
				}
			}
			/*!
			 * @return O elemento no inicio da lista. Caso a lista esteja vazia, retorna um std::range_error.
			*/
			const T &front() const {
				if(m_head->next != m_tail) {
					Node *target = m_head->next;
					return target->data;
				}
				// Caso a lista esteja vazia.
				else {
					throw std::range_error("List is empty!");//
				}
			}
			/*!
			 * Substitui o conteudo da lista com uma certa quantiade de uma determinado valor.
			 * @param count Quantidade de elementos que terão na lista.
			 * @param value Valor que será inserido em toda a lista.
			*/
			void assign(const size_type &count, const T &value) {
				clear();
				for(size_type i = 0; i < count; i++) {
					push_back(value);
				}
			}
			/*!
			 * Substituí o conteudo da lista com o conteudo de uma lista inicializadora.
			 * @param ilist Lista inicializadora.
			*/
			void assign(std::initializer_list<T> ilist) {
				clear();
				auto ilistTemp = ilist.begin();
				while(ilistTemp != ilist.end()) {
					push_back(*ilistTemp);
					ilistTemp++;
				}
			}

			/* Operations with iterators */

			/*!
			 * @return Um iterator para o inicio da lista.
			*/
			iterator begin() {
				return m_head->next; // Retorna um iterator para o elemento seguinte ao m_head.
			}
			/*!
			 * @return Um iterator para o final da lista. m_tail.
			*/
			iterator end() {
				return m_tail;
			}
			/*!
			 * @return Um iterator constante para o inicio da lista. Primeiro elemento.
			*/
			const_iterator cbegin() const {
				return m_head->next;
			}
			/*!
			 * @return Um iterator constante para o final da lista. Após o ultimo elemento.
			*/
			const_iterator cend() const {
				return m_tail;
			}

			/* Insert */

			/* Iterator versions */

			/*!
			 * Insere um elemento antes da posição pos.
			 * @param pos Iterator para a posição a qual o elemento adicionado irá precedela.
			 * @param value Valor a qual será adicionado na lista.
			 * @return Um iterator para a posição em que o elemento foi adicionado.
			*/
			iterator insert(iterator pos, const T &value) {
				Node *temp = pos.current; // Posição do iterator.
				Node *aux = temp->prev;
				aux->next = new Node(value, aux, temp);
				temp->prev = aux->next;
				m_size++;
				return aux->next;
			}
			/*!
			 * Insere um range de elementos antes da posição pos.
			 * @param pos Posição a qual os elementos adicionados irão presceder.
			 * @param first Inicio do range de elementos.
			 * @param last Final do range de elementos.
			 * @return Iterator para o ultimo elemento do range na lista.
			*/
			template<typename InItr>
			iterator insert(iterator pos, InItr first, InItr last) {
				while(first != last) {
					insert(pos, *first);
					first++;
				}
				return pos;
			}
			/*!
			 * Insere uma lista inicializadora de elementos antes da posição pos.
			 * @param pos Posição a qual os elementos adicionados irão presceder.
			 * @param ilist Lista inicializadora.
			 * @return Iterator para o ultimo da lista inicializadora na lista.
			*/
			iterator insert(iterator pos, std::initializer_list<T> ilist) {
				auto ilistTemp = ilist.begin();
				while(ilistTemp != ilist.end()) {
					insert(pos, *ilistTemp);
					ilistTemp++;
				}
				return pos;
			}

			/* Const_Iterator versions */
			
			/*!
			 * Insere um elemento antes da posição pos.
			 * @param pos Iterator para a posição a qual o elemento adicionado irá precedela.
			 * @param value Valor a qual será adicionado na lista.
			 * @return Um iterator para a posição em que o elemento foi adicionado.
			*/
			const_iterator insert(const_iterator pos, const T &value) {
				Node *temp = pos.current; // Posição do const_iterator.
				Node *aux = temp->prev;
				aux->next = new Node(value, aux, temp);
				temp->prev = aux->next;
				m_size++;
				return aux->next;
			}
			/*!
			 * Insere um range de elementos antes da posição pos.
			 * @param pos Posição a qual os elementos adicionados irão presceder.
			 * @param first Inicio do range de elementos.
			 * @param last Final do range de elementos.
			 * @return const_iterator para o ultimo elemento do range na lista.
			*/
			template<typename InItr>
			const_iterator insert(const_iterator pos, InItr first, InItr last) {
				while(first != last) {
					insert(pos, *first);
					first++;
				}
				return pos;
			}
			/*!
			 * Insere uma lista inicializadora de elementos antes da posição pos.
			 * @param pos Posição a qual os elementos adicionados irão presceder.
			 * @param ilist Lista inicializadora.
			 * @return const_iterator para o ultimo da lista inicializadora na lista.
			*/
			const_iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
				auto ilistTemp = ilist.begin();
				while(ilistTemp != ilist.end()) {
					insert(pos, *ilistTemp);
					ilistTemp++;
				}
				return pos;
			}

			/* Fim Insert */

			/* Erase */

			/* Iterator versions */

			/*!
			 * Apaga um elemento da lista dado pela posição pos.
			 * @param pos Iterator para a posiçao a qual o elemento será apagado.
			 * @return Elemento seguinte a pos antes de ser apagado.
			*/
			iterator erase(iterator pos) {
				Node *target = pos.current; // Salva a posição atual.
				Node *prevTemp = target->prev; // Salva o nó que o antecede.
				Node *nextTemp = target->next; // Salva o nó que o prescede.
				delete target; 
				prevTemp->next = nextTemp; // Liga o nó que o sucedia ao que o prescendia.
				nextTemp->prev = prevTemp; // Liga o nó que o prescendia ao que o sucedia. 
				m_size--; // Diminui o tamanho da lista.
				return nextTemp; // Nó seguinte a pós antes de ser apagado.
			}
			/*!
			 * Remove os elementos da lista que estão no range dado.
			 * @param first Iterator para o inicio do range.
			 * @param last Iterator para o final do range.
			 * @return Elemento seguinte ao range apagado.
			*/
			iterator erase(iterator first, iterator last) {
				while(first != last) {
					first = erase(first);
				}
				return last;
			}

			/* Const_Iterator versions */

			/*!
			 * Apaga um elemento da lista dado pela posição pos.
			 * @param pos Iterator para a posiçao a qual o elemento será apagado.
			 * @return Elemento seguinte a pos antes de ser apagado.
			*/
			const_iterator erase(const_iterator pos) {
				Node *target = pos.current; // Salva a posição atual.
				Node *prevTemp = target->prev; // Salva o nó que o antecede.
				Node *nextTemp = target->next; // Salva o nó que o prescede.
				delete target; 
				prevTemp->next = nextTemp; // Liga o nó que o sucedia ao que o prescendia.
				nextTemp->prev = prevTemp; // Liga o nó que o prescendia ao que o sucedia. 
				m_size--; // Diminui o tamanho da lista.
				return nextTemp; // Nó seguinte a pós antes de ser apagado.
			}
			/*!
			 * Remove os elementos da lista que estão no range dado.
			 * @param first const_iterator para o inicio do range.
			 * @param last const_iterator para o final do range.
			 * @return Elemento seguinte ao range apagado.
			*/
			const_iterator erase(const_iterator first, const_iterator last) {
				while(first != last) {
					first = erase(first);
				}
				return last;
			}

			/* Fim erase */

			/*!
			 * Procura um determinado valor na lista.
			 * @param value Valor a ser procurado.
			 * @return const_iterator para a posição em que value se encontra na lista. Caso não seja encotnrado, será retornado o endereço do final da lista.
			*/
			const_iterator find(const T &value) const {
				const_iterator avance = m_head->next; // Inicia o avance na posição do primeiro elemento.
				while(avance != m_tail) {
					// Caso encontre o valor na lista, retornar um iterator para a sua posição.
					if(*avance == value) {
						return avance;
					}
					avance++;
				}
				return m_tail;
			}

			/* Operadores de comparação */

			/*!
			 * Compara duas listas.
			 * @return true caso sejam iguais e false caso contrario.
			*/
			bool operator==(const list &other) {
				// Verificar se as listas possuem tamanhos diferentes. Caso sim, elas são diferentes.
				if(other.m_size != m_size) {
					return false;
				}
				// Caso tenham o mesmo tamanho, comparar todos os nós para verificar se possuem elementos diferentes.
				Node *temp1 = m_head;
				Node *temp2 = other.m_head;

				while(temp1 != m_tail) {
					temp1 = temp1->next;
					temp2 = temp2->next;
					// Caso encontre algum valor diferente na mesma posição que na outra lista, elas são diferentes.
					if(temp1->data != temp2->data) {
						return false;
					}
				}
				// Caso passe por todos os testes acima e não tenha sido invalidado a igualdade elas são iguais.
				return true;
			}
			/*!
			 * Compara duas listas.
			 * @return true caso sejam diferentes e false caso contrario.
			*/
			bool operator!=(const list &other) {
				return !(*this == other); // Caso as listas não sejam iguais elas são diferentes.
			}
			/*! Operador de impressão. */
			friend std::ostream &operator<<(std::ostream &out, const list<T> &l) {
				Node *it = l.m_head->next;
				while(it != l.m_tail) {
					out << it->data << " ";
					it = it->next;
				}
				return out;
			}

		private:
			size_type m_size = 0; // Toda lista criada tem inicialmente tamanho zero.
			Node *m_head; // Ponteiro nó cabeça.
			Node *m_tail; // Ponteiro nó calda.
						
	}; // Final classe list.

} // Final namespace.

#endif