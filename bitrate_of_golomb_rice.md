## Expected bit rate of Golomb-Rice code for Geometric distribution

The bit rate $R_b(\delta)$ of a Golomb-Rice code with Rice parameter $b$ for the codeword $\delta$ can be calculated as follows:

$$ R_b(\delta) = b + 1 + \lfloor \frac{\delta}{2^b} \rfloor $$

The expectation value is: $E[R_b] = E[b + 1 + \lfloor \frac{\delta}{2^b} \rfloor]$

By linearity of the [expectation value](https://en.wikipedia.org/wiki/Expected_value#Properties):

$$ E[R_b] = E[b + 1 + \lfloor \frac{\delta}{2^b} \rfloor] = E[b] + E[1] + E[\lfloor \frac{\delta}{2^b} \rfloor] $$

$b$ and $1$ are constants, so:

$$ E[R_b] = b + 1 + E[\lfloor \frac{\delta}{2^b} \rfloor] $$

$E[\lfloor \frac{\delta}{2^b} \rfloor]$ can be calculated as follows:

$$ E[\lfloor \frac{\delta}{2^b} \rfloor] = \sum_{j=0}^{\infty}{\lfloor \frac{j}{2^b} \rfloor \cdot p(\delta=j)} $$

assuming a [Geometric distribution](https://en.wikipedia.org/wiki/Geometric_distribution) with parameter $\alpha$ for chance of failure and $j$ failures until success for the probability, gives


$$ = \sum_{j=0}^{\infty}{\lfloor \frac{j}{2^b} \rfloor \cdot (1-\alpha)\alpha^j} $$

extracting constant terms out of the sum and getting rid of the floor function gives

$$ = (1-\alpha) \sum_{l=0}^{\infty}\sum_{j=l\cdot2^b}^{(l+1)\cdot2^b-1}{l \alpha^j} = (1-\alpha) \sum_{l=0}^{\infty}l\cdot\sum_{j=l\cdot2^b}^{(l+1)\cdot2^b-1}{\alpha^j} $$

rearranging the second sum

$$ = (1-\alpha) \sum_{l=0}^{\infty}l\cdot(\sum_{j=0}^{(l+1)\cdot2^b-1}{\alpha^j} - \sum_{j=0}^{l\cdot2^b-1}{\alpha^j)} $$

with $\sum_{j=0}^{n}{\alpha^j} = \frac{1-\alpha^{n+1}}{1-\alpha}$ [(see)](https://en.wikipedia.org/wiki/Geometric_series#Convergence_of_the_series_and_its_proof) and here $n = (l+1)\cdot2^b-1$ or $n = l\cdot2^b-1$ we get

$$ = (1-\alpha) \sum_{l=0}^{\infty}l\cdot(\frac{1-\alpha^{(l+1)2^b}}{1-\alpha} - \frac{1-\alpha^{l \cdot 2^b}}{1-\alpha}) $$

$$ = \sum_{l=0}^{\infty}l\cdot((1-\alpha^{(l+1)2^b}) - (1-\alpha^{l \cdot 2^b})) $$

$$ = \sum_{l=0}^{\infty}l\cdot(1-\alpha^{l \cdot 2^b}\alpha^{2^b} - 1 + \alpha^{l \cdot 2^b}) $$

$$ = \sum_{l=0}^{\infty}l\cdot(\alpha^{l \cdot 2^b}-\alpha^{l \cdot 2^b}\alpha^{2^b}) $$

$$ = \sum_{l=0}^{\infty}l \alpha^{l \cdot 2^b}\cdot(1-\alpha^{2^b}) = (1-\alpha^{2^b}) \cdot \sum_{l=0}^{\infty}l \alpha^{l \cdot 2^b} $$

$$ = (1-\alpha^{2^b}) \cdot \sum_{l=0}^{\infty}l {(\alpha^{2^b})}^{l} $$

with $\sum_{l=0}^{\infty}lx^l = \frac{x}{(1-x)^2}$ [(see)](https://math.stackexchange.com/questions/647587/sum-of-a-power-series-n-xn) and here $x = \alpha^{2^b} $

$$ = (1-\alpha^{2^b}) \cdot \frac{\alpha^{2^b}}{(1-\alpha^{2^b})^2} $$

$$ = \frac{\alpha^{2^b}}{1-\alpha^{2^b}} = E(\lfloor \frac{\delta}{2^b} \rfloor) $$

with this result, the full expectation value is

$$ E[R_b] = E[b + 1 + \lfloor \frac{\delta}{2^b} \rfloor] = b + 1 + E[\lfloor \frac{\delta}{2^b} \rfloor] = b + 1 + \frac{\alpha^{2^b}}{1-\alpha^{2^b}} $$

expanding the 1 to simplify further

$$ = b + \frac{1-\alpha^{2^b}}{1-\alpha^{2^b}} + \frac{\alpha^{2^b}}{1-\alpha^{2^b}} = b + \frac{1-\alpha^{2^b}+\alpha^{2^b}}{1-\alpha^{2^b}} $$

and finally (under a Geometric distribution with probability of failure $\alpha$)

$$ E[R_b] = b + \frac{1}{1-\alpha^{2^b}} $$

In our case we have $\alpha = 1-\frac{1}{2^b}$ (because we have a fraction of $\frac{1}{2^b}$ nodes that can be "hit" by an edge) and therefore

$$ E[R_b] = b + \frac{1}{1-(1-\frac{1}{2^b})^{2^b}} $$

and for large numbers $2^b$, with the definition of the [exp-function](https://en.wikipedia.org/wiki/Exponential_function#Limit_of_integer_powers), we finally get

$$ E(R_b) \approx b + \frac{1}{1-e^{-1}} \approx b + 1.58 $$

