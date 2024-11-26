#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
                       real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex& other) const {
        if (other.imag == 0 && other.real == 0){
            throw invalid_argument("Ошибка: деление на ноль.");

        }
        double denominator = other.real * other.real + other.imag * other.imag;
        return Complex((real * other.real + imag * other.imag) / denominator,
                       (imag * other.real - real * other.imag) / denominator);
    }

    double modulus() const {
        return sqrt(real * real + imag * imag);
    }

    double argument() const {
        return atan2(imag, real);
    }

    void display() const {
        cout << real << (imag >= 0 ? "+" : "") << imag << "i" << endl;
    }
};

int main() {
    Complex z1(3, 4);
    Complex z2;

    cout << "z1 = ";
    z1.display();
    cout << "z2 = ";
    z2.display();

    Complex sum = z1 + z2;
    cout << "Сумма z1 + z2 = ";
    sum.display();

    Complex diff = z1 - z2;
    cout << "Разность z1 - z2 = ";
    diff.display();

    Complex product = z1 * z2;
    cout << "Произведение z1 * z2 = ";
    product.display();
    try{
        Complex quotient = z1 / z2;
        cout << "Частное z1 / z2 = ";
        quotient.display();
    } catch (const runtime_error& e){
        cerr << "Caught an exception: " << e.what() << endl;
    }


    cout << "Модуль z1 = " << z1.modulus() << endl;
    cout << "Аргумент z1 = " << z1.argument() << " рад" << endl;

    return 0;
}