#include <cstring>
#include <iostream>
#include <stdexcept>

struct Varf {
    int x, y;

    bool operator==(const Varf &) const = default;
};

std::istream &operator>>(std::istream &in, Varf &v) { return in >> v.x >> v.y; }

std::ostream &operator<<(std::ostream &out, const Varf &v) {
    return out << '(' << v.x << ", " << v.y << ')';
}

class Poligon {
  public:
    Poligon() : Poligon(nullptr, 0, "", false) {}

    Poligon(Varf *varfuri, int num_varfuri, const char *culoare)
        : Poligon(varfuri, num_varfuri, culoare, true) {}

    Poligon(const Poligon &p)
        : Poligon(p.m_varfuri, p.m_num_varfuri, p.m_culoare, false) {}
    
    

    bool operator==(const Poligon &p) const {
        if (p.m_num_varfuri != m_num_varfuri) {
            return false;
        }
        if (strcmp(m_culoare, p.m_culoare) != 0) {
            return false;
        }
        for (int i = 0; i < m_num_varfuri; i++) {
            if (m_varfuri[i] != p.m_varfuri[i]) {
                return false;
            }
        }
        return true;
    };

    bool operator!=(const Poligon &) const = default;

    Poligon &operator-=(int index) {
        if (index < 0 || index >= m_num_varfuri) {
            throw std::out_of_range("out of index");
        }

        for (int i = index + 1; i < m_num_varfuri; i++) {
            m_varfuri[i - 1] = m_varfuri[i];
        }

        m_num_varfuri--;

        return *this;
    }

    const char *culoare() const noexcept { return m_culoare; }
    int numar_varfuri() const noexcept { return m_num_varfuri; }
    const Varf *varfuri() const noexcept { return m_varfuri; }

    ~Poligon() {
        delete[] m_varfuri;
        delete[] m_culoare;
    }

  private:
    Poligon(Varf *varfuri, int num_varfuri, const char *culoare,
            bool must_sort) {
        m_varfuri = new Varf[num_varfuri];
        for (int i = 0; i < num_varfuri; i++) {
            m_varfuri[i] = varfuri[i];
        }
        m_num_varfuri = num_varfuri;

        m_culoare = new char[strlen(culoare) + 1];
        strcpy(m_culoare, culoare);

        for (int i = 0; (i < num_varfuri - 1) && must_sort; i++) {
            for (int j = i + 1; j < num_varfuri; j++) {
                Varf *a = m_varfuri + i;
                Varf *b = m_varfuri + j;
                if (b->x < a->x || (b->x == a->x && b->y < a->y)) {
                    std::swap(*a, *b);
                }
            }
        }
    }

    Varf *m_varfuri;
    int m_num_varfuri;
    char *m_culoare;
};

std::ostream &operator<<(std::ostream &out, const Poligon &p) {
    out << (p.numar_varfuri() == 4 ? "Patrulaterul" : "Poligonul") << " este "
        << p.culoare() << " si are varfurile ";
    for (int i = 0; i < p.numar_varfuri(); i++) {
        if (i > 0) {
            out << ", ";
        }
        out << p.varfuri()[i];
    }
    return out;
}

bool contine_varf_si_e_de_culare(const Poligon &p, Varf v,
                                 const char *culoare) {
    if (strcmp(p.culoare(), culoare) != 0) {
        return false;
    }

    for (int i = 0; i < p.numar_varfuri(); i++) {
        if (p.varfuri()[i] == v) {
            return true;
        }
    }

    return false;
}