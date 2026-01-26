# Chapter 2

## 2.1

$$
\sum_{k=0}^{K} a_k y^{(k)}(t) = \sum_{l=0}^{L} b_l x^{(l)}(t) \\\\
H(s) = \frac{Y(s)}{X(s)} = \frac{\sum_{l=0}^{L} b_l s^l}{\sum_{k=0}^{K} a_k s^k} = \frac{b_L}{a_K} \cdot \\\\ \frac{\prod_{l=1}^{L}(s - z_l)}{\prod_{k=1}^{K}(s - p_k)}
$$

### From factored form get pole/zero plots
- Poles enhance/increase response (increase in sensitivity)
- Zeros suppress/reduce response (decrease in sensitivity)

## 2.2

$$
y(t) = x(t) * h(t) \tag{1} \\\\
Y(\omega) = X(\omega)H(\omega)
$$

- IIR -- Infinite Impulse Response (causal)
    - Like a bad relative (never shuts up)
    - Most well developed filter out there
### Distortionless transmission

$$
y(t) = ax(t-t_0) \implies H(\omega) = ae^{-j\omega t_g} \\\\
|H(\omega)| = |a| \\\\
\angle H(\omega) = -\omega t_g \\\\
t_g = -\frac{d}{d\omega} \angle H(\omega) \quad \text{(group delay)}
$$


### Hilbert transformer

$$
h(t) = \frac{1}{\pi t} \implies H(\omega) = -jsgn(\omega)
$$

- Comes down to phase on this
- Same energy very different look
