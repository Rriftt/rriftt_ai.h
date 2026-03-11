```c
float rai_randn(float mean, float std_dev);
```

* Pseudorandom numbers drawn from a Gaussian distribution centred around `mean` and having standard deviation `std_dev`.
* Seeded by `RAI_SRAND()`.
* Depends on `RAI_RAND()` for uniformly distributed pseudorandom number generation.
* Employs **Box-Muller transform** on the uniform random numbers to generate normally distributed random numbers.
