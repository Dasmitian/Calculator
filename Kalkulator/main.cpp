#include <iostream>
#include <string>
#include <unordered_map>

char allowedChars[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 40, 41, 42, 43, 45, 46, 47, 94};
std::unordered_map <int, std::string> operations = { {42, "Multiplication"}, {94, "Exponentiation"}, {43, "Addition"}, {45, "Substraction"}, {47, "Divide"} };
int error = 0;

int CheckString(std::string equation) {
	for (int i = 0; i <= equation.length(); i++) {
		for (int j = 0; j <= sizeof(allowedChars); j++) {
			if (allowedChars[j] == equation[i]) {
				break;
			}
			if (j == sizeof(allowedChars)) {
				return 1;
			}
		}
	}
	return 0;
}

static std::string Calculation(std::string equation, std::vector<std::string> calculations, std::string operation, char operationSymbol) {
	int pos = std::distance(calculations.begin(), std::find(calculations.begin(), calculations.end(), operation));
	double firstNumber = std::stod(calculations[pos - 1]);
	double secondNumber = std::stod(calculations[pos + 1]);
	std::string firstNumberString = calculations[pos - 1];
	std::string secondNumberString = calculations[pos + 1];
	std::string calculation;
	std::string equationToReplace;
	switch (operationSymbol) {
	case '^':
		calculation = std::to_string(double(std::pow(firstNumber, secondNumber)));
		equationToReplace = firstNumberString + "^" + secondNumberString;
		break;
	case '*': 
		calculation = std::to_string(firstNumber * secondNumber);
		equationToReplace = firstNumberString + "*" + secondNumberString;
		break;
	case '/':
		calculation = std::to_string(firstNumber / secondNumber);
		equationToReplace = firstNumberString + "/" + secondNumberString;
		break;
	case '+':
		calculation = std::to_string(firstNumber + secondNumber);
		equationToReplace = firstNumberString + "+" + secondNumberString;
		break;
	case '-':
		calculation = std::to_string(firstNumber - secondNumber);
		equationToReplace = firstNumberString + "-" + secondNumberString;
		break;
	};
	equation.replace(equation.find(equationToReplace), equationToReplace.size(), calculation);
	return equation;
}

static std::string Calculate(std::string subEquation) {
	std::string numberString;
	std::vector<std::string> calculations;
	for (int i = 0; i <= subEquation.length(); i++){
		char symbol = subEquation[i];
		if (symbol >= 48 && symbol <= 57 || symbol == 46) {
			numberString += symbol;
		}
		std::unordered_map<int, std::string>::const_iterator operation = operations.find(symbol);
		if (operation != operations.end()) {
			calculations.insert(calculations.end(), numberString);
			calculations.insert(calculations.end(), operation->second);
			numberString = "";
		}
		if (i == subEquation.length()) {
			calculations.insert(calculations.end(), numberString);
		}
	}
	if (std::find(calculations.begin(), calculations.end(), "Exponentiation") != calculations.end()) {
		subEquation = Calculation(subEquation, calculations, "Exponentiation", '^');
		subEquation = Calculate(subEquation);
	} else if (std::find(calculations.begin(), calculations.end(), "Multiplication") != calculations.end()) {
		subEquation = Calculation(subEquation, calculations, "Multiplication", '*');
		subEquation = Calculate(subEquation);
	} else if (std::find(calculations.begin(), calculations.end(), "Divide") != calculations.end()) {
		subEquation = Calculation(subEquation, calculations, "Divide", '/');
		subEquation = Calculate(subEquation);
	} else if (std::find(calculations.begin(), calculations.end(), "Addition") != calculations.end()) {
		subEquation = Calculation(subEquation, calculations, "Addition", '+');
		subEquation = Calculate(subEquation);
	} else if (std::find(calculations.begin(), calculations.end(), "Substraction") != calculations.end()) {
		subEquation = Calculation(subEquation, calculations, "Substraction", '-');
		subEquation = Calculate(subEquation);
	} else {
		return subEquation;
	}
	return subEquation;
}

std::string CalculateBrackets(std::string equation) {
	int openBracketCount = 0;
	int closeBracketCount = 0;
	int openBracketPos = 0;
	int closeBracketPos = 0;
	std::string bracketEquation;
	for (int i = 0; i <= equation.size(); i++) {
		char symbol = equation[i];

		if (symbol == 40) {
			openBracketCount++;
			openBracketPos = i;
			if (equation[i + 1] == 41) {
				std::cout << "Missing equation in brackets" << std::endl;
				error = 1;
				break;
			}
		}
		if (symbol == 41) {
			closeBracketCount++;
			closeBracketPos = i;
			if (closeBracketCount > openBracketCount) {
				std::cout << "Missing open bracket" << std::endl;
				error = 1;
				break;
			}
			bracketEquation = equation.substr(openBracketPos+1, closeBracketPos - openBracketPos-1);
			std::string calculatedBrackets = Calculate(bracketEquation);
			equation.replace(equation.find(bracketEquation) - 1, bracketEquation.size() + 2, calculatedBrackets);
			equation = CalculateBrackets(equation);
			break;
		}
		if (i == equation.size() && openBracketCount > closeBracketCount) {
			std::cout << "Missing close bracket" << std::endl;
			error = 1;
			break;
		}
	}
	return equation;
}

int main() {
	std::string equation = "(1234.22+2+12-30)*4^2*(23*45.0005)/12-44";
	if (CheckString(equation)) {
		std::cout << "Invalid string" << std::endl;
	}
	else {
		std::cout << equation << std::endl;
		equation = CalculateBrackets(equation);
		if (error == 0) {
			equation = Calculate(equation);
			std::cout << "Result: " << equation << std::endl;
		} else {
			std::cout << "Incorrect equation." << std::endl;
		}
	}
	std::cin.get();
}