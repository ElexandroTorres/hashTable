#ifndef ACCOUNT_H
#define ACCOUNT_H

struct Account {
	using AcctKey = int;

	std::string m_name;
	short m_bank;
	int m_agency;
	int m_acount;
	float m_balance;

	int getKey() {
		return m_acount;
	}

	friend std::ostream &operator<<(std::ostream &out, const Account &acc) {
		out << "| Nome: " << acc.m_name << " | Banco: " << acc.m_bank << " | Agencia: " << acc.m_agency << " | Conta: " 
		    << acc.m_acount << " | Saldo: " << acc.m_balance << " |\n";
				
		return out;
	}

};

bool operator==(const Account &lhs, const Account &rhs) {
    	return lhs.m_name == rhs.m_name and lhs.m_bank == rhs.m_bank
    	   and lhs.m_agency == rhs.m_agency and lhs.m_acount == rhs.m_acount
    	   and lhs.m_balance == rhs.m_balance;
}

// Versão 1.
struct KeyHash {
	std::size_t operator()(const Account::AcctKey &k_) const {
		return std::hash<int>()(k_);
	}
};
// Versão 1.
struct  KeyEqual{
	bool operator()(const Account::AcctKey &lhs_, const Account::AcctKey &rhs_) const {
		return lhs_ == rhs_;
	}
};

#endif
