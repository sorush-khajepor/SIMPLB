#ifndef SARRAY_H
#define SARRAY_H

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
                    s = s + data[i];
                }
                return s;
        }

        virtual const T computeVol() const {
                T v = T(1);
                for (int i=0;i<N;i++){
                    v = v * data[i];
                }
                return v;
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


        friend std::ostream& operator<<(std::ostream& os, const SArray& arr){
            for (int i=0;i<N;i++){
                os<<std::endl<<"data("<<i<<") = "<<arr[i];
            }
            os<<std::endl<<std::endl;
            return os;
        }
};

#endif
