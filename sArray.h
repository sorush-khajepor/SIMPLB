#ifndef SARRAY_H
#define SARRAY_H


#include "lattice.h"

#include<iostream>

template <class T, int N>
class SArray{

protected:
        T data[N];

public:
        SArray(){
            *this=0.;
        }
        SArray(const T rhs[]){
            *this=rhs;
        }

        SArray(const T rhs){
            *this=rhs;
        }
        SArray(std::initializer_list<T> rhs) {
          std::copy(rhs.begin(), rhs.end(), data);
        }

        template<class U>
        operator SArray<U,N>()  {
            SArray<U,N> arr;
            for (int i=0;i<N;i++){
                arr[i] = data[i];
            }
            return arr;
        }

        virtual SArray& operator= (const T& rhs){
                for (int i=0;i<N;i++){
                    data[i] = rhs;
                }
                return *this;
            }

        virtual SArray& operator= (const T rhs[]){
                for (int i=0;i<N;i++){
                    data[i] = rhs[i];
                }
                return *this;
            }

        virtual void copyTo (T lhs[]) const {
                for (int i=0;i<N;i++){
                    lhs[i]=data[i];
                }
        }

        virtual SArray& operator= (const SArray& rhs){
                for (int i=0;i<N;i++){
                    data[i] = rhs[i];
                }
                return *this;
            }
        virtual SArray operator+ (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] + rhs.data[i];
                }
                return arr;
            }
        virtual SArray operator+ (const T& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] + rhs;
                }
                return arr;
            }
        virtual SArray operator- (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] - rhs.data[i];
                }
                return arr;
            }
        virtual SArray operator- (const T& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] - rhs;
                }
                return arr;
            }
        virtual SArray operator* (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] * rhs.data[i];
                }
                return arr;
            }
        virtual SArray operator* (const T& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] * rhs;
                }
                return arr;
            }
        virtual SArray operator/ (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] / rhs.data[i];
                }
                return arr;
            }

        virtual SArray operator/ (const T& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] / rhs;
                }
                return arr;
            }
            
        virtual const T sum() const {
                T s = T();
                for (int i=0;i<N;i++){
                    s += data[i];
                }
                return s;
        }

        virtual const T dot(const SArray& arr){
            return (*this * arr).sum();
        }

        T& operator[](const int& i){
            return data[i];
        }
        const T& operator[] (const int& i)const{
            return data[i];
        }

        virtual void print () {
            for (int i=0;i<N;i++){
                std::cout<< "data("<<i<<") = "<<data[i]<< std::endl;
            }
        }
};
/*
// Lattice vector
template <class T>
class ArrayNQ :public SArray<T, lattice::nQ>{

};

// Cartesian vector
template <class T>
class ArrayND :public SArray<T, lattice::nD>{

    public:

    using SArray<T,lattice::nD>::SArray;
    using SArray<T,lattice::nD>::operator=;

    template<class U>
    operator ArrayND<U>()  {
        ArrayND<U> arr;
        for (int i=0;i<lattice::nD;i++){
            arr[i] = this->data[i];
        }
        return arr;
    }

    T dot(const ArrayND& arr){
        return (*this * arr).sum();
    }
};
*/
#endif
