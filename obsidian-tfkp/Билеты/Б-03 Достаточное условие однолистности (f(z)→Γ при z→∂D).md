---
tags: [билет, тфкп]
aliases: [Б-03]
номер: 3
---
# Б-03. Достаточное условие однолистности голоморфной непостоянной функции с условием $f(z)\to\Gamma$ при $z\to\partial D$

> [!question] Формулировка билета
> Достаточное условие однолистности голоморфной непостоянной в данной области $D$ функции $f$ с условием $f(z)\to\Gamma$ при $z\to\partial D$, где $\Gamma$ — замкнутая жорданова кривая.

**Где в лекциях:** гл. 13, слайды 276–280 (Предл. 10.3 — напоминание, Предл. 13.3); Предл. 10.3 — гл. 10, слайд 230.

*Обозначения:* в лекциях область определения — $\Omega$, а $D=D(\Gamma)=\mathrm{Int}(\Gamma)$ — образ; в программе область названа $D$.

## Кратко (план ответа)
- Обратный принцип соответствия границ: формулировка и доказательство через принцип аргумента и индекс жордановой кривой.
- Формулировка Предл. 13.3; смысл условия $f(z)\to\Gamma$; необходимость непостоянства $f$.
- $\partial f(\Omega)\subset\Gamma$ (компактность $\widehat{\mathbb C}$ + открытость $f(\Omega)$); затем $f(\Omega)\subset D$ и $f(\Omega)=D$ по топологической лемме.
- $\widetilde\Omega$ — точки «однократности»; $\widetilde\Omega\ne\varnothing$; берём $z_0\in\Omega\cap\partial\widetilde\Omega$.
- Пары $z_n\neq w_n$ с $f(z_n)=f(w_n)$; $w_0\in\Omega$ (иначе $f(z_0)\in\Gamma$); если $w_0\ne z_0$ — открытость; если $w_0=z_0$ — $f'(z_0)=0$, затем Руше: $f$ не менее чем двулистна около $z_0$.
- Итог: $f$ — гомеоморфизм $\Omega\to D$, значит $\Omega$ односвязна. Примеры: Кэли, Бляшке, $z^2$, $\text{Ж}$ в кольце.

---

## Определения
> [!info] Расширенная плоскость и голоморфность в $\infty$
> $\widehat{\mathbb C}=\mathbb C\cup\{\infty\}$ (сфера Римана) — компактное метрическое пространство; окрестности $\infty$: $\{|z|>R\}\cup\{\infty\}$. Для $\Omega\subset\widehat{\mathbb C}$ замыкание $\overline\Omega$ и граница $\partial\Omega$ берутся в $\widehat{\mathbb C}$. $f$ *голоморфна в $\infty$*, если $f(\infty)\in\mathbb C$ и $w\mapsto f(1/w)$ голоморфна в окрестности $w=0$ (с $f(1/0):=f(\infty)$). $H(\Omega)$ для $\Omega\subset\widehat{\mathbb C}$ — функции, голоморфные в каждой точке $\Omega$ (в частности, конечнозначные).

> [!info] Условие «$f(z)\to\Gamma$ при $z\to\partial\Omega$»
> Если $z_n\in\Omega$ и все предельные точки последовательности $\{z_n\}$ (в $\widehat{\mathbb C}$) лежат на $\partial\Omega$, то все предельные точки последовательности $\{f(z_n)\}$ (в $\widehat{\mathbb C}$) лежат на $\Gamma$. Для ограниченной $\Gamma$ это равносильно: $\operatorname{dist}(f(z),\Gamma)\to0$, когда $z$ стремится к $\partial\Omega$ (в сферической метрике).

> [!info] Жорданова кривая и теорема Жордана
> Замкнутая жорданова кривая — гомеоморфный образ окружности. $\widehat{\mathbb C}\setminus\Gamma=D\sqcup D_\infty$, где $D=D(\Gamma)=\mathrm{Int}(\Gamma)$ — ограниченная компонента, $\infty\in D_\infty=\mathrm{Ext}(\Gamma)$, и $\partial D=\partial D_\infty=\Gamma$. Индекс параметризации $\Gamma$ равен $0$ в $D_\infty$ и $\pm1$ (один знак) в $D$ (подробнее — [[Б-02 Обратный принцип соответствия границ. Теорема Носиро–Варшавского|Б-02]]).

> [!info] Кратность значения
> Значение $w$ принимается функцией $f$ в точке $z_0\in\Omega\cap\mathbb C$ с кратностью $k$, если $z_0$ — нуль порядка $k$ функции $f-w$. «$f$ принимает значение $w$ ровно один раз с учётом кратности»: существует единственная точка $z_w\in\Omega$ с $f(z_w)=w$, и в ней $f'(z_w)\ne0$.

> [!info] Односвязность
> Область $\Omega\subset\widehat{\mathbb C}$ односвязна, если любой замкнутый путь в $\Omega$ гомотопен в $\Omega$ постоянному. Для областей в $\widehat{\mathbb C}$ это равносильно связности $\widehat{\mathbb C}\setminus\Omega$ (факт первого семестра). Односвязность сохраняется при гомеоморфизмах.

## Формулировка
> [!important] Предложение 10.3 (обратный принцип соответствия границ)
> Пусть $D\subset\mathbb C$ — жорданова область, $\Gamma=\partial D$, и $f\in H(D)\cap C(\overline D)$ взаимно однозначна на $\Gamma$. Тогда $f\in\mathcal U(D)$ и $f$ отображает $D$ конформно на область $\mathrm{Int}(f(\Gamma))$.

> [!important] Предложение 13.3 (условие однолистности)
> Пусть $\Omega\subset\widehat{\mathbb C}$ — область, $\Omega\ne\widehat{\mathbb C}$, $\Gamma$ — замкнутая жорданова кривая в $\mathbb C$ и $D=D(\Gamma)$. Пусть функция $f\in H(\Omega)$ **непостоянна** и $f(z)\to\Gamma$ при $z\to\partial\Omega$. Тогда $f(\Omega)=D$.
> Более того, если $f$ принимает в $\Omega$ какое-то значение из $D$ ровно один раз (с учётом кратности), то $f$ инъективна (т.е. $f\in\mathrm{Conf}(\Omega,D)$), а $\Omega$ односвязна.

> [!important] Лемма А (топологическая)
> Пусть $X$ — топологическое пространство, $A\subset X$, $E\subset X$ связно, $E\cap A\ne\varnothing$ и $E\setminus A\ne\varnothing$. Тогда $E\cap\partial A\ne\varnothing$.

**Связь с Предл. 13.3.** В условиях Предл. 10.3 выполнено $f(z)\to\Upsilon=f(\Gamma)$ при $z\to\partial D$: если предельные точки $\{z_n\}\subset D$ лежат на $\Gamma$, то для любой подпоследовательности $z_{n_k}\to\zeta\in\Gamma$ по непрерывности $f$ в $\overline D$ имеем $f(z_{n_k})\to f(\zeta)\in\Upsilon$. Поэтому утверждение $f(D)=\mathrm{Int}\,\Upsilon$ — частный случай Предл. 13.3. Предл. 13.3 гибче: не требуется ни непрерывности $f$ вплоть до границы, ни жордановости (и даже односвязности) $\Omega$; односвязность получается как *следствие*.

## Доказательство
### Предложение 10.3 (обратный принцип соответствия границ)
Пусть $\gamma:[0,1]\to\Gamma$ — положительно ориентированная параметризация $\Gamma$. Тогда $\sigma=f\circ\gamma$ непрерывна, замкнута и инъективна на $[0,1)$, т.е. $\Upsilon=f(\Gamma)$ — замкнутая жорданова кривая; пусть $G=\mathrm{Int}(\Upsilon)$, $E=\mathrm{Ext}(\Upsilon)$. Для $b\notin\Upsilon$ функция $f-b\in H(D)\cap C(\overline D)$ не обращается в нуль на $\Gamma$, и по усиленному [[Принцип аргумента и теорема Руше|принципу аргумента]] (Теорема 10.2)
$$z(f-b,D)=\tfrac1{2\pi}\Delta_{\partial^+D}\operatorname{Arg}(f-b)=\tfrac1{2\pi}\Delta_\sigma\operatorname{Arg}(w-b)=\operatorname{Ind}(\sigma,b).$$
По дополнению к теореме Жордана $\operatorname{Ind}(\sigma,b)=0$ при $b\in E$ и $\operatorname{Ind}(\sigma,b)=\varepsilon\in\{\pm1\}$ (одно и то же для всех $b\in G$) при $b\in G$; так как число нулей неотрицательно, $\varepsilon=1$. Итак, точки $E$ не принимаются в $D$, а каждая точка $G$ принимается ровно один раз и с кратностью $1$.

Точки $b\in\Upsilon$ тоже не принимаются: $f$ непостоянна (она инъективна на $\Gamma$), поэтому $f(D)$ открыто, и если бы $b=f(z_0)\in\Upsilon$, $z_0\in D$, то $f(D)$ содержало бы круг $B(b,\varepsilon)$, а он пересекает $E$ (так как $b\in\partial E$) — противоречие. Значит, $f$ — биекция $D$ на $G$, и по Предл. 13.1 $f\in\mathrm{Conf}(D,G)$. (Подробнее, включая гомеоморфизм замыканий, — [[Б-02 Обратный принцип соответствия границ. Теорема Носиро–Варшавского|Б-02]].) $\square$

### Лемма А
Пусть $E\cap\partial A=\varnothing$. Пространство $X$ есть дизъюнктное объединение $\mathrm{int}A$, $\partial A$ и $\mathrm{ext}A=X\setminus\overline A$. Тогда $E=(E\cap\mathrm{int}A)\sqcup(E\cap\mathrm{ext}A)$ — объединение двух относительно открытых в $E$ множеств. Точка из $E\cap A$ лежит в $\mathrm{int}A$ (в $\partial A$ она не лежит), а точка из $E\setminus A$ лежит в $\mathrm{ext}A$; значит, оба множества непусты, что противоречит связности $E$. $\square$

> [!note]- Деталь: сведение к случаю $\Omega\subset\mathbb C$
> Если $\infty\in\Omega$, выберем $p\in\widehat{\mathbb C}\setminus\Omega$ (существует, так как $\Omega\ne\widehat{\mathbb C}$); $p\ne\infty$. ДЛО $T(z)=1/(z-p)$ — гомеоморфизм $\widehat{\mathbb C}$ на себя, $T(p)=\infty$, поэтому $\Omega'=T(\Omega)\subset\mathbb C$ — область, $T(\partial\Omega)=\partial\Omega'$, и $T$ сохраняет сходимость последовательностей и их предельные точки. Функция $f_1=f\circ T^{-1}$ голоморфна в $\Omega'$ (в точке $T(\infty)=0$ — по определению голоморфности $f$ в $\infty$, так как $T^{-1}(w)=p+1/w$), непостоянна, $f_1(\Omega')=f(\Omega)$, $f_1(z)\to\Gamma$ при $z\to\partial\Omega'$; число прообразов каждого значения с учётом кратности у $f$ и $f_1$ одинаково (кратность в $\infty$ определяется через локальную координату, а $T$ локально конформна). Наконец, инъективность $f$ и односвязность $\Omega$ равносильны инъективности $f_1$ и односвязности $\Omega'$.
>
> Поэтому **далее считаем $\Omega\subset\mathbb C$.**

### Предл. 13.3: первое утверждение $f(\Omega)=D$
По принципу сохранения области $f(\Omega)$ — область в $\mathbb C$; в частности, $f(\Omega)$ открыто, и $\partial f(\Omega)\cap f(\Omega)=\varnothing$.

**Шаг 1: $\partial f(\Omega)\subset\Gamma$** (граница в $\widehat{\mathbb C}$). Пусть $w\in\partial f(\Omega)$ (случай $w=\infty$ не исключается). Найдутся $z_n\in\Omega$ с $f(z_n)\to w$. По компактности $\widehat{\mathbb C}$, переходя к подпоследовательности, считаем $z_n\to\zeta\in\overline\Omega$. Если $\zeta\in\Omega$, то $f(z_n)\to f(\zeta)$ по непрерывности, т.е. $w=f(\zeta)\in f(\Omega)$, что невозможно, так как $w\in\partial f(\Omega)$ и $f(\Omega)$ открыто. Значит, $\zeta\in\partial\Omega$; у $\{z_n\}$ единственная предельная точка $\zeta\in\partial\Omega$, и по условию все предельные точки $\{f(z_n)\}$, т.е. $w$, лежат на $\Gamma$. (В частности, $\infty\notin\partial f(\Omega)$, и $f$ ограничена.)

**Шаг 2: $f(\Omega)\subset D$.** Пусть $f(\Omega)\not\subset D$. Тогда $f(\Omega)$ пересекает $\Gamma$ или $D_\infty$. Если $f(\Omega)\ni b\in\Gamma$, то, так как $f(\Omega)$ открыто, а $b\in\Gamma=\partial D_\infty$, множество $f(\Omega)$ пересекает $D_\infty$. Итак, в любом случае $D_\infty\cap f(\Omega)\ne\varnothing$. С другой стороны, $\infty\in D_\infty\setminus f(\Omega)$ ($f$ конечнозначна). Множество $D_\infty$ связно, и по Лемме А $D_\infty\cap\partial f(\Omega)\ne\varnothing$. Но $\partial f(\Omega)\subset\Gamma$ и $\Gamma\cap D_\infty=\varnothing$ — противоречие.

**Шаг 3: $f(\Omega)=D$.** Если $D\setminus f(\Omega)\ne\varnothing$, то связное множество $D$ пересекает $f(\Omega)$ (по шагу 2 $\varnothing\neq f(\Omega)\subset D$) и его дополнение, и по Лемме А $D\cap\partial f(\Omega)\neq\varnothing$, что противоречит $\partial f(\Omega)\subset\Gamma$, $\Gamma\cap D=\varnothing$. $\square$

### Предл. 13.3: второе утверждение (однолистность и односвязность)
Положим
$$\widetilde\Omega=\bigl\{z\in\Omega:\ f'(z)\ne0\ \text{и}\ f(z)\ne f(z')\ \text{для всех}\ z'\in\Omega,\ z'\ne z\bigr\}$$
— множество точек, в которых $f$ «однократна». По условию существует $a\in D$, принимаемое ровно один раз с учётом кратности: $f(z_a)=a$ для единственной $z_a\in\Omega$, и $f'(z_a)\ne0$. Значит, $z_a\in\widetilde\Omega\ne\varnothing$. Докажем, что $\widetilde\Omega=\Omega$; тогда $f$ инъективна и $f'\ne0$ в $\Omega$.

Пусть $\Omega\setminus\widetilde\Omega\neq\varnothing$. Так как $\Omega$ связна, по Лемме А (с $X=\mathbb C$, $E=\Omega$, $A=\widetilde\Omega$) найдётся $z_0\in\Omega\cap\partial\widetilde\Omega$. Каждая окрестность $z_0$ содержит точки $\widetilde\Omega$; получим противоречие, построив окрестность $z_0$ без точек $\widetilde\Omega$.

> [!note]- Деталь: существование $z_n\in\Omega\setminus\widetilde\Omega$, $z_n\to z_0$
> Либо $z_0\notin\widetilde\Omega$, и можно взять $z_n=z_0$; либо $z_0\in\widetilde\Omega$, и тогда любая окрестность $z_0$ содержит точки $\mathbb C\setminus\widetilde\Omega$ (так как $z_0\in\partial\widetilde\Omega$), которые при малой окрестности лежат в $\Omega$ ($\Omega$ открыто).

Точка $z_n\notin\widetilde\Omega$ означает: либо (i) $f'(z_n)=0$, либо (ii) существует $w_n\in\Omega$, $w_n\ne z_n$, $f(w_n)=f(z_n)$.

**Случай I: (i) выполнено для бесконечного числа $n$.** Тогда по непрерывности $f'$ получаем $f'(z_0)=0$ — переходим к шагу «$f'(z_0)=0$» ниже.

**Случай II: (ii) выполнено для всех $n$, начиная с некоторого.** По компактности $\widehat{\mathbb C}$ можно считать $w_n\to w_0\in\overline\Omega$. Так как $f(w_n)=f(z_n)\to f(z_0)\in f(\Omega)=D$, точка $w_0$ не может лежать на $\partial\Omega$: иначе по условию $f(z)\to\Gamma$ (применённому к $\{w_n\}$ с единственной предельной точкой $w_0\in\partial\Omega$) предел $f(z_0)$ лежал бы на $\Gamma$, а $\Gamma\cap D=\varnothing$. Итак, $w_0\in\Omega$ и $f(w_0)=f(z_0)$.

*Подслучай II.1: $w_0\ne z_0$.* Возьмём непересекающиеся круги $U_0=B(z_0,r)$, $V_0=B(w_0,r)$, лежащие в $\Omega$. Функция $f$ непостоянна на каждом из них (иначе по теореме единственности $f$ постоянна в $\Omega$), поэтому по принципу сохранения области $f(U_0)$ и $f(V_0)$ — открытые множества, содержащие $c=f(z_0)=f(w_0)$. Значит, найдётся $\varepsilon>0$ с $W=B(c,\varepsilon)\subset f(U_0)\cap f(V_0)$. Окрестность $N=U_0\cap f^{-1}(W)$ точки $z_0$ открыта (непрерывность $f$). Любая точка $\zeta\in N$ не лежит в $\widetilde\Omega$: значение $f(\zeta)\in W\subset f(V_0)$ принимается и в некоторой точке $\zeta'\in V_0$, а $\zeta'\ne\zeta$, так как $U_0\cap V_0=\varnothing$. Итак, $N\cap\widetilde\Omega=\varnothing$ — противоречие с $z_0\in\partial\widetilde\Omega$.

*Подслучай II.2: $w_0=z_0$.* Покажем, что $f'(z_0)=0$. Пусть $\varepsilon>0$. Возьмём $\rho>0$ с $B(z_0,\rho)\subset\Omega$ и $|f'(\zeta)-f'(z_0)|<\varepsilon$ при $\zeta\in B(z_0,\rho)$ (непрерывность $f'$). Так как $z_n\to z_0$ и $w_n\to z_0$, найдётся $N$ с $z_N,w_N\in B(z_0,\rho)$; тогда $[z_N,w_N]\subset B(z_0,\rho)$ и
$$0=f(w_N)-f(z_N)=\int_{[z_N,w_N]}\bigl(f'(\zeta)-f'(z_0)\bigr)\,d\zeta+f'(z_0)(w_N-z_N).$$
Отсюда $|f'(z_0)|\,|w_N-z_N|\leqslant\varepsilon|w_N-z_N|$, и, поскольку $w_N\ne z_N$, $|f'(z_0)|\leqslant\varepsilon$. В силу произвольности $\varepsilon$ имеем $f'(z_0)=0$.

**Шаг «$f'(z_0)=0$» (завершает случаи I и II.2).** Так как $f$ непостоянна, $f(z)-f(z_0)=(z-z_0)^kg(z)$ в некотором круге $B(z_0,\rho)\subset\Omega$, где $g\in H(B(z_0,\rho))$, $g(z_0)\neq0$, и $k\geqslant2$ (так как $f'(z_0)=0$). Выберем $\delta\in(0,\rho)$ так, что $g\ne0$ на $\overline B(z_0,\delta)$, и положим
$$\varepsilon=\delta^k\min_{|z-z_0|=\delta}|g(z)|>0 .$$
Пусть $w\in B(f(z_0),\varepsilon)$. На окружности $|z-z_0|=\delta$ имеем $|f(z)-f(z_0)|=\delta^k|g(z)|\geqslant\varepsilon>|f(z_0)-w|$. По [[Принцип аргумента и теорема Руше|теореме Руше]] функция $f(z)-w=\bigl(f(z)-f(z_0)\bigr)+\bigl(f(z_0)-w\bigr)$ имеет в $B(z_0,\delta)$ столько же нулей (с учётом кратности), сколько $f(z)-f(z_0)=(z-z_0)^kg(z)$, т.е. $k\geqslant2$.

Теперь пусть $\zeta\in N:=B(z_0,\delta)\cap f^{-1}(B(f(z_0),\varepsilon))$ — это окрестность $z_0$. Для $w=f(\zeta)$ функция $f-w$ имеет в $B(z_0,\delta)$ не менее двух нулей с учётом кратности, один из которых $\zeta$. Значит, либо $\zeta$ — кратный нуль, т.е. $f'(\zeta)=0$, либо есть ещё нуль $\zeta'\ne\zeta$, т.е. $f(\zeta')=f(\zeta)$. В обоих случаях $\zeta\notin\widetilde\Omega$. Итак, $N\cap\widetilde\Omega=\varnothing$ — противоречие с $z_0\in\partial\widetilde\Omega$.

Все случаи приводят к противоречию, значит, $\widetilde\Omega=\Omega$: $f$ инъективна в $\Omega$ и $f'\ne0$, т.е. (по Предл. 13.1, [[Б-01 Однолистные функции. Критерий локальной однолистности|Б-01]]) $f\in\mathrm{Conf}(\Omega,D)$, и $f:\Omega\to D$ — гомеоморфизм.

**Односвязность $\Omega$.** Область $D=\mathrm{Int}(\Gamma)$ односвязна: $\widehat{\mathbb C}\setminus D=\Gamma\cup D_\infty=\overline{D_\infty}$ (так как $\partial D_\infty=\Gamma$) — замыкание связного множества, а значит, связно. Пусть $\gamma$ — замкнутый путь в $\Omega$; $f\circ\gamma$ — замкнутый путь в $D$, гомотопный в $D$ постоянному посредством гомотопии $H(s,t)$; тогда $f^{-1}\circ H$ — гомотопия $\gamma$ к постоянному пути в $\Omega$ (непрерывна, так как $f^{-1}$ непрерывна). Следовательно, $\Omega$ односвязна. $\square$

> [!note]- Другой взгляд: «степень» отображения
> Доказательство фактически показывает, что $f:\Omega\to D$ — *собственное* отображение (прообраз компакта $K\subset D$ компактен в $\Omega$: иначе нашлась бы последовательность $z_n\in f^{-1}(K)$ с предельной точкой на $\partial\Omega$, и $f(z_n)$ имели бы предельные точки на $\Gamma$, а не в $K$). Для собственного непостоянного голоморфного отображения число прообразов $n(w)$ точки $w\in D$ с учётом кратности конечно и локально постоянно (по теореме Руше), следовательно, постоянно в связной $D$. Условие «некоторое значение принимается ровно один раз» означает $n\equiv1$, т.е. инъективность. Выше этот факт получен «вручную» через $\widetilde\Omega$.

## Примеры и замечания
1. **Преобразование Кэли.** $\Omega=\{\operatorname{Im} z>0\}$, $f(z)=\frac{z-i}{z+i}$, $\Gamma=\mathbb T$, $D=\mathbb D$. $f$ — ДЛФ, непрерывная на $\overline\Omega\subset\widehat{\mathbb C}$ (полюс $-i\notin\overline\Omega$), $|f(x)|=\frac{|x-i|}{|x+i|}=1$ при $x\in\mathbb R$ и $f(\infty)=1$, т.е. $|f|=1$ на $\partial\Omega=\mathbb R\cup\{\infty\}$. Поэтому $f(z)\to\mathbb T$ при $z\to\partial\Omega$. Значение $0$ принимается только в $z=i$, и $f'(i)=\frac{2i}{(2i)^2}=\frac1{2i}\ne0$. По Предл. 13.3 $f$ конформно отображает верхнюю полуплоскость на $\mathbb D$. Здесь $\infty\in\partial\Omega$ — случай, не охватываемый Предл. 10.3.
2. **Конечные произведения Бляшке.** $B(z)=e^{i\theta}\prod_{j=1}^n\frac{z-a_j}{1-\overline{a_j}z}$, $a_j\in\mathbb D$. $B\in C(\overline{\mathbb D})$, $|B|=1$ на $\mathbb T$, поэтому $B(z)\to\mathbb T$ при $|z|\to1$, и $B(\mathbb D)=\mathbb D$. Значение $0$ принимается $n$ раз с учётом кратности; при $n=1$ Предл. 13.3 даёт однолистность ($B\in\operatorname{CAut}(\mathbb D)$).
3. **Условие «ровно один раз» существенно.** $f(z)=z^2$ в $\mathbb D$: $f(z)\to\mathbb T$, $f(\mathbb D)=\mathbb D$, но каждое $w\ne0$ принимается дважды, а $0$ — один раз, но с кратностью $2$; $f$ не однолистна.
4. **Неодносвязная $\Omega$.** $\Omega=\{\frac12<|z|<2\}$, $f=\text{Ж}$, $\text{Ж}(z)=\frac12(z+\frac1z)$. Обе граничные окружности переходят в эллипс $\Gamma=\{\frac{u^2}{(5/4)^2}+\frac{v^2}{(3/4)^2}=1\}$ (при $z=2e^{it}$: $\text{Ж}=\frac54\cos t+\frac{3i}4\sin t$; при $z=\frac12e^{it}$: $\text{Ж}=\frac54\cos t-\frac{3i}4\sin t$), и по непрерывности $f$ в $\overline\Omega$ выполнено $f(z)\to\Gamma$. По Предл. 13.3 $f(\Omega)=\mathrm{Int}\,\Gamma$. Однолистности нет: $\text{Ж}(z)=\text{Ж}(1/z)$, и $z,1/z\in\Omega$ одновременно; каждое значение принимается дважды (с учётом кратности) — согласуется с неодносвязностью кольца.

> [!warning] Замечания к слайдам
> - Слайд 278: не оговорены условия «$f$ непостоянна» и «$\Omega\ne\widehat{\mathbb C}$», а они необходимы: постоянная $f\equiv c\in\Gamma$ удовлетворяет $f(z)\to\Gamma$, но $f(\Omega)=\{c\}\ne D$; при $\Omega=\widehat{\mathbb C}$ граница пуста, условие бессодержательно, а $f\in H(\widehat{\mathbb C})$ постоянна по Лиувиллю. (В программе непостоянство указано явно.)
> - Слайд 279: во фразе «так как $\partial\Omega\subset\Gamma$» имеется в виду $\partial f(\Omega)\subset\Gamma$.

---

## Методы доказательства
- [[Топологические рассуждения (связность, компактность)]] — Лемма А, компактность $\widehat{\mathbb C}$, $\partial f(\Omega)\subset\Gamma$, граничная точка $z_0$ множества $\widetilde\Omega$, односвязность через гомеоморфизм.
- [[Принцип аргумента и теорема Руше]] — Предл. 10.3 (индекс жордановой кривой) и двулистность около критической точки (Руше).
- [[Принцип соответствия границ]] — Предл. 10.3 и его обобщение Предл. 13.3.
- [[Нормировка дробно-линейными отображениями и корнем]] — сведение к $\Omega\subset\mathbb C$ ДЛО $T(z)=1/(z-p)$.

## Связанные билеты
- [[Б-02 Обратный принцип соответствия границ. Теорема Носиро–Варшавского|Б-02]] — Предл. 10.3 с полным доказательством (включая гомеоморфизм замыканий).
- [[Б-01 Однолистные функции. Критерий локальной однолистности|Б-01]] — Предл. 13.1 и локальная $n$-листность в критической точке.
- [[Б-00 Предварительные сведения и глава 12|Б-00]] — принцип аргумента, теорема Руше, открытость, произведения Бляшке.
- [[Б-07 Группы автоморфизмов D, C, Ĉ|Б-07]] — пример Бляшке при $n=1$: автоморфизмы круга.
