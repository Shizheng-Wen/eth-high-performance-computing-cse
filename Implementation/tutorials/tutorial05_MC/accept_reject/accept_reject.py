import numpy as np
import scipy 
import matplotlib.pyplot as plt

np.random.seed(42)

def f(x):
    return np.exp(-x*x/2)*(np.sin(6 + x)**2 + 3*np.cos(x)**2 * np.sin(4*x)**2 + 1)

def h(x, sigma=1.0, gamma=0.0):
    return np.exp(-0.5*((x - gamma)/sigma)**2)/np.sqrt(2*np.pi)

def main():

    # Plot f(x)
    x = np.linspace(-4 , 4, 1000)

    fig, (ax1, ax2) = plt.subplots(1, 2)

    ax1.plot(x, f(x))
    ax2.plot(x, f(x))

    #plt.savefig("accept_reject.png")
    #return

    ###############################
    # Accept-reject using uniform distr
    ###############################
    
    f_max = np.max(f(x))

    a = np.linspace(-3, 3, 1000)
    b = np.ones_like(a)*f_max

    b[0] = 0.001
    b[-1] = 0.001
    ax1.plot(a, b)

    #plt.savefig("accept_reject.png")
    #return

    n = int(1e6) #number of random samples

    x = np.random.uniform(-3, 3, size=n)
    u = np.random.uniform(size=n)

    ax1.plot(x, u*f_max, '.')

    #plt.savefig("accept_reject.png")
    #return

    u_inside = u[u < f(x)/f_max]
    x_inside = x[u < f(x)/f_max]

    ax1.plot(x_inside, u_inside*f_max, 'r.')

    #plt.savefig("accept_reject.png")
    #return


    accepted = u_inside.shape[0]/n
    sampling_area = 6*f_max
    area = accepted*sampling_area

    print("acceptance rate: ", accepted)
    print("sampling_area: ", sampling_area)
    print("area: ", area)
    print()

    #plt.savefig("accept_reject.png")
    #return

    ###############################
    #lets do accept-reject sampling
    ###############################
    
    lambda_ = 10.0
    sigma = 1.0
    h_ = lambda_*h(u)
    ax2.plot(a, h(a)*lambda_)

    #plt.savefig("accept_reject.png")
    #return

    x = np.random.normal(size=n)                    # x ~ N(0,1)
    u = np.random.uniform(size=n)

    slide = np.logical_and(x>-3, x<3)
    x = x[slide]
    u = u[slide]

    n = x.shape[0] # update n

    ax2.plot(x, u*lambda_*h(x), '.')

    #plt.savefig("accept_reject.png")
    #return

    u_inside = u[u < f(x)/(lambda_*h(x))]
    x_inside   = x[u < f(x)/(lambda_*h(x))]

    ax2.plot(x_inside, u_inside*lambda_*h(x_inside), 'r.')

    #plt.savefig("accept_reject.png")
    #return
    
    u_accepted = u_inside.shape[0]/n
    sampling_area = -scipy.special.erf(-3.0/(sigma*np.sqrt(2)))
    sampling_area *= lambda_
    area = u_accepted*sampling_area

    print("accpetance rate: ", u_accepted)
    print("sampling_area: ", sampling_area)
    print("area: ", area)


    ###############################
    # finalize
    ###############################

    #plt.show()
    plt.savefig("accept_reject.png")
    return

if __name__ == "__main__":
    main()