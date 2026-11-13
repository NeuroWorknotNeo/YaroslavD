import numpy as np, matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
OUT='/srv/claude/workspace/chat_m1004446490379_20/YaroslavD/phase_portraits/'
import os; os.makedirs(OUT,exist_ok=True)
INK='#1f2328'; GRAY='#8a9099'; SEP='#c2410c'; TRAJ='#2b6cb0'

def draw(fname,title,f,U,xr,yr,eqs,seps,levels):
    fig,(a1,a2)=plt.subplots(2,1,figsize=(8,10),gridspec_kw={'height_ratios':[1,2.4]},sharex=True)
    xs=np.linspace(*xr,1200)
    a1.plot(xs,U(xs),color=INK,lw=2)
    for x0,kind in eqs: a1.plot(x0,U(x0),'o',ms=8,color=SEP if kind!='центр' else TRAJ,mec='white',mew=1.5)
    for E in seps: a1.axhline(E,color=SEP,lw=1,ls='--')
    a1.set_ylabel('U(x)'); a1.set_title(title,fontsize=13)
    a1.grid(alpha=.25); 
    lo,hi=U(xs).min(),U(xs).max(); a1.set_ylim(max(lo,levels[0]*1.3)-0.3, min(hi,levels[-1]*1.2)+0.5)
    X,Y=np.meshgrid(np.linspace(*xr,600),np.linspace(*yr,600))
    E=Y**2/2+U(X)
    a2.contour(X,Y,E,levels=levels,colors=TRAJ,linewidths=1,linestyles='solid')
    a2.contour(X,Y,E,levels=seps,colors=SEP,linewidths=2.2,linestyles='solid')
    Xs,Ys=np.meshgrid(np.linspace(*xr,40),np.linspace(*yr,40))
    a2.streamplot(Xs,Ys,Ys,f(Xs),density=0.9,color='#b0b5bc',linewidth=0.5,arrowsize=1.1)
    for x0,kind in eqs:
        a2.plot(x0,0,'o',ms=9,color=SEP if kind!='центр' else TRAJ,mec='white',mew=1.5,zorder=5)
        a2.annotate(kind,(x0,0),xytext=(6,8),textcoords='offset points',fontsize=10,color=INK,
                    bbox=dict(fc='white',ec='none',alpha=.8,pad=1))
    a2.set_xlim(*xr); a2.set_ylim(*yr); a2.set_xlabel('x'); a2.set_ylabel('y = ẋ')
    a2.axhline(0,color=GRAY,lw=.5); a2.axvline(0,color=GRAY,lw=.5)
    a2.plot([],[],color=SEP,lw=2.2,label='сепаратрисы'); a2.plot([],[],color=TRAJ,lw=1,label='линии уровня энергии y²/2+U(x)')
    a2.legend(loc='lower right',fontsize=9,framealpha=.9)
    fig.tight_layout(); fig.savefig(OUT+fname,dpi=150); plt.close(fig)

f1=lambda x: x**5-5*x**4+6*x**3
U1=lambda x: -(x**6/6-x**5+1.5*x**4)
draw('eq1_phase.png',r"1) $\ddot x = x^5-5x^4+6x^3 = x^3(x-2)(x-3)$",f1,U1,(-1.2,3.8),(-4,4),
     [(0,'вырожд. седло'),(2,'центр'),(3,'седло')],[0.0],
     sorted(set(np.r_[np.linspace(-2.6,-0.2,7),np.linspace(0.6,6,7)])))
f2=lambda x: 4*x**3-2*x
U2=lambda x: -x**4+x**2
r=1/np.sqrt(2)
draw('eq2_phase.png',r"2) $\ddot x = 4x^3-2x$",f2,U2,(-1.6,1.6),(-1.6,1.6),
     [(-r,'седло'),(0,'центр'),(r,'седло')],[0.25],
     sorted(set(np.r_[np.linspace(0.03,0.22,5),np.linspace(0.32,1.2,6),np.linspace(-1.5,-0.1,5)])))
print('ok')
