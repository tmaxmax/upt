#include <iostream>

struct Complex {
    double r, i;

    Complex operator+(Complex c) { return {r + c.r, i + c.i}; }
    Complex operator*(Complex c) {
        return {r * c.r - i * c.i, r * c.i + i * c.r};
    }
};

std::istream &operator>>(std::istream &in, Complex &c) {
    return in >> c.r >> c.i;
}

std::ostream &operator<<(std::ostream &out, const Complex &c) {
    return out << c.i << ',' << c.i;
}

class Polinom {
  public:
    Polinom(Complex *coefs, int coefs_len) {
        m_coefs = new Complex[coefs_len];
        for (int i = 0; i < coefs_len; i++) {
            m_coefs[i] = coefs[i];
        }
        m_grad = coefs_len - 1;
    }

    Polinom(const Polinom &to_copy) {
        m_coefs = new Complex[to_copy.m_grad + 1];
        for (int i = 0; i <= to_copy.m_grad; i++) {
            m_coefs[i] = to_copy.m_coefs[i];
        }
        m_grad = to_copy.m_grad;
    };

    Polinom &operator=(const Polinom &to_copy) {
        if (m_grad != to_copy.m_grad) {
            delete[] m_coefs;
            m_coefs = new Complex[to_copy.m_grad + 1];
            m_grad = to_copy.m_grad;
        }

        for (int i = 0; i <= to_copy.m_grad; i++) {
            m_coefs[i] = to_copy.m_coefs[i];
        }

        return *this;
    }

    Complex eval(Complex z) {
        Complex res{};
        for (int i = m_grad; i >= 0; i--) {
            res = res * z + m_coefs[i];
        }
        return res;
    }

    Polinom(Polinom &&) = delete;
    Polinom &operator=(Polinom &&) = delete;

    ~Polinom() { delete[] m_coefs; }

  private:
    Complex *m_coefs;
    int m_grad;
};

int main() {
    // Input format:
    // 2
    // 2 0.5 1.0
    // 1 -3.0 1.0
    // 0 5.0 4.0
    // 10.0
    int grad;
    std::cin >> grad;

    Complex *arr = new Complex[grad + 1];
    for (int i = 0; i <= grad; i++) {
        int g;
        std::cin >> g;
        std::cin >> arr[g].r >> arr[g].i;
    }

    Complex z{};
    std::cin >> z;

    Polinom p(arr, grad + 1);
    Complex r = p.eval(z);

    std::cout << r << '\n';

    delete[] arr;

    return 0;
}