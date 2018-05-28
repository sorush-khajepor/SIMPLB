#ifndef CONTAINER_H
#define CONTAINER_H


#include "lattice.h"

#include<iostream>
/*
// A class containing only data array with the length of number of lattice velocities (nQ)
template <class T>
class arrayNQ{
        T data[lattice::nQ];
public:
        arrayNQ(){
            *this=0.;
        }
        arrayNQ(const T rhs[]){
            *this=rhs;
        }

        arrayNQ& operator= (const T& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs;
                }
            }
        arrayNQ& operator= (const T rhs[]){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        arrayNQ& operator= (const arrayNQ& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        arrayNQ& operator= (const Node& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        void copyTo(Node& lhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                     lhs[iQ]=data[iQ];
                }
        }
        T& operator[](const int& iQ){
            return data[iQ];
        }
        const T& operator[] (const int& iQ)const{
            return data[iQ];
        }

        void print () {
            for (int iQ=0;iQ<lattice::nQ;iQ++){
                std::cout<< "data("<<iQ<<") = "<<data[iQ]<< std::endl;
            }
        }
};


// An array containing only cartesian data.
template <class T>
class arrayND{
        T data[lattice::nD];
public:
        // Initialized with zero
        arrayND(){
            *this=0.;
        }

        // Initialized with a standard C array
        arrayND(const T rhs[]){
            *this=rhs;
        }

        // Filling array with one value
        arrayND& operator= (const T& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs;
                }
            }
        // Filling array with another arrayND
        arrayND& operator= (const arrayND& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        // Filling array with another standard c array
        arrayND& operator= (const T rhs[]){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        // An array member is called by its index
        T& operator[](const int& iD){
            return data[iD];
        }

        // Const call to an array member
        const T& operator[] (const int& iD)const{
            return data[iD];
        }

        // Print array on screen
        void print () {
            for (int iD=0;iD<lattice::nD;iD++){
                std::cout<< "data("<<iD<<") = "<<data[iD]<< std::endl;
            }
        }
};

*/

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

        SArray& operator= (const T& rhs){
                for (int i=0;i<N;i++){
                    data[i] = rhs;
                }
                return *this;
            }

        SArray& operator= (const T rhs[]){
                for (int i=0;i<N;i++){
                    data[i] = rhs[i];
                }
                return *this;
            }

        void copyTo (const T lhs[]){
                for (int i=0;i<N;i++){
                    lhs[i]=data[i];
                }
        }


        SArray& operator= (const SArray& rhs){
                for (int i=0;i<N;i++){
                    data[i] = rhs[i];
                }
                return *this;
            }
        SArray operator+ (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] + rhs.data[i];
                }
                return arr;
            }
        SArray operator- (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] - rhs.data[i];
                }
                return arr;
            }
        SArray operator* (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] * rhs.data[i];
                }
                return arr;
            }
        SArray operator/ (const SArray& rhs) const {
                SArray arr;
                for (int i=0;i<N;i++){
                    arr.data[i] = this->data[i] / rhs.data[i];
                }
                return arr;
            }

        const T sum() const {
                T s = T();
                for (int i=0;i<N;i++){
                    s += data[i];
                }
                return s;
        }

        T& operator[](const int& i){
            return data[i];
        }
        const T& operator[] (const int& i)const{
            return data[i];
        }

        void print () {
            for (int i=0;i<N;i++){
                std::cout<< "data("<<i<<") = "<<data[i]<< std::endl;
            }
        }
};

template <class T>
class ArrayNQ :public SArray<T, lattice::nQ>{

};

template <class T>
class ArrayND :public SArray<T, lattice::nD>{

};

#endif
