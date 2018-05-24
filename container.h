#include "lattice.h" 
#include<iostream>


// A class containing only data vector with the length of lattice vectors.
// array with the length of lattice directions (nQ)
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
        arrayND(){
            *this=0.;
        }

        arrayND(const T rhs[]){
            *this=rhs;
        }

        arrayND& operator= (const T& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs;
                }
            }
        arrayND& operator= (const arrayND& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        arrayND& operator= (const T rhs[]){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        T& operator[](const int& iD){
            return data[iD];
        }
        const T& operator[] (const int& iD)const{
            return data[iD];
        }

        void print () {
            for (int iD=0;iD<lattice::nD;iD++){
                std::cout<< "data("<<iD<<") = "<<data[iD]<< std::endl;
            }
        }
};
