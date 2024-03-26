function [R, x] = gauss_solve(A, b)
    n = length(A);

    for j = 1 : n - 1
        if A(j, j) = 0
            error("zero pivot encountered");
        end

        for i = j + 1 : n
            coef = A(i, j) / A(j, j);
            for k = j + 1 : n
                A(i, k) = A(i, k) - coef * A(j, k);
            end
            b(i) = b(i) - coef * b(j);
        end
    end

    x = zeros(1, n);

    for i = n : -1 : 1
        for j = i + 1 : n
            b(i) = b(i) - A(i, j) * x(j);
        end
        x(i) = b(i) / A(i, i);
    end

    R = A;
end