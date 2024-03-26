function x = fpi_zero(f, x0, prec, max_iter)
    if ~exist('max_iter', 'var') || isempty(max_iter)
        max_iter = 128;
    end
    # By having a delta between iterations of an order of magnitude
    # less than the desired number of precise digits we
    # ensure that we actually get that much precision.
    tol = 10 ^ (-prec - 1); 

    x = x0;

    for i = 1:max_iter
        xi = f(x);
        delta = xi - x;
        x = xi;
        # We take the maximum absolute delta because
        # 1. x may also be a vector.
        # 2. by taking the maximum it is ensured that
        #    all other errors are in the expected
        #    tolerance interval.
        if max(abs(delta)) < tol
            return
        end
    end

    if isvector(x)
        x = [];
    else
        x = NaN;
    end
end