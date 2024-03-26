function x = jacobi(A, x0, b, prec)
    D = diag(diag(A));
    L = tril(A) - D;
    U = triu(A) - D;

    x = fpi_zero(@(x) inv(D) * (b - (L + U) * x), x0, prec);
end