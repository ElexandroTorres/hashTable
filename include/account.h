#ifndef ACCOUNT_H
#define ACCOUNT_H

/*!
 * Estrutura que representa uma conta.
 * Foi implementada uma versão simples que utilizar o numero da conta como chave.
*/
struct Account {
	using AcctKey = int; //<! Definição do tipo AccKey

	std::string m_name; //<! Nome do cliente.
	short m_bank; //<! Numero do banco.
	int m_agency; //<! Numero da Agencia.
	int m_acount; //<! Numero da conta.
	float m_balance; //<! Saldo.

	/// Versão 1. Retorna o numero da conta como sendo a chave.
	AcctKey getKey() {
		return m_acount;
	}
	/// Sobrecarga do operador de extração.
	friend std::ostream &operator<<(std::ostream &out, const Account &acc) {
		out << "| Nome: " << acc.m_name << " | Banco: " << acc.m_bank << " | Agencia: " << acc.m_agency << " | Conta: " 
		    << acc.m_acount << " | Saldo: " << acc.m_balance << " |\n";
				
		return out;
	}

};
/// Sobrecarga do operador de igualdade, para permitir comparar duas contas.
bool operator==(const Account &lhs, const Account &rhs) {
    	return lhs.m_name == rhs.m_name and lhs.m_bank == rhs.m_bank
    	   and lhs.m_agency == rhs.m_agency and lhs.m_acount == rhs.m_acount
    	   and lhs.m_balance == rhs.m_balance;
}

/// KeyHash Versão 1.
struct KeyHash {
	std::size_t operator()(const Account::AcctKey &k_) const {
		return std::hash<int>()(k_);
	}
};
/// KeyEqual Versão 1.
struct  KeyEqual{
	bool operator()(const Account::AcctKey &lhs_, const Account::AcctKey &rhs_) const {
		return lhs_ == rhs_;
	}
};

#endif
