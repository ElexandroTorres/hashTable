#include <iostream>
#include <cmath>


/*!
 * Verifica se um numero é primo.
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
size_t nextPrime(size_t &number) {
	if(number <= 1) {
		return 1;
	}
	//Procurar proximos primos, começando de number para verificar se ele é primo.
	auto aux = number;
	while(is_prime(aux) == false) {
		aux++;
	}

	return aux;
}

int main(void) {
	
	size_t numero = 2;
	auto result = nextPrime(numero);

	std::cout << "Proximo primo: " << result << std::endl;

	return 0;

}