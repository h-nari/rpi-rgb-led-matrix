#ifndef _timeval_h_
#define _timeval_h_

#include <sys/time.h>


class TimeVal {
 protected:
  struct timeval m_tv;

 public:
  inline time_t       sec() const { return m_tv.tv_sec;}
  inline suseconds_t usec() const { return m_tv.tv_usec;}
  
  TimeVal() {}
  
  TimeVal(const TimeVal &t) {
    m_tv.tv_sec  = t.sec();
    m_tv.tv_usec = t.usec();
  }
  
  TimeVal(TimeVal *p) {
    m_tv.tv_sec  = p->sec();
    m_tv.tv_usec = p->usec();
  }

  
  void normalize(){
    int sec;
    if(usec() < 0){
      sec = (999999 - usec()) / 1000000;
      m_tv.tv_sec -= sec;
      m_tv.tv_usec += sec * 1000000;
    } else if(usec() > 1000000){
      sec = usec() / 1000000;
      m_tv.tv_sec  += sec;
      m_tv.tv_usec -= sec * 1000000;
    }
  }
  
  static TimeVal &now() {
    TimeVal *t = new TimeVal;
    if(gettimeofday(&t->m_tv, NULL) < 0) {
      fprintf(stderr,"gettimeofday failed.\n");
      exit(1);
    }
    return *t;
  }

  TimeVal& operator+=(int usec)
  {
    m_tv.tv_usec += usec;
    normalize();
    return *this;
  }

  TimeVal operator+(int usec)
  {
    TimeVal v(this);
    v += usec;
    return v;
  }

  int compare(TimeVal &obj){
    TimeVal a(this);
    TimeVal b(obj);
    a.normalize();
    b.normalize();
    if(a.sec() != b.sec())
      return a.sec() > b.sec();
    else
      return a.usec() > b.usec();
  }

  int operator>(TimeVal b){
    return this->compare(b);
  }
  
  int operator<(TimeVal b){
    return b.compare(*this);
  }

  long operator-(TimeVal b){
    long d = this->sec() - b.sec();
    d = d * 1000000 + this->usec() - b.usec();
    return d;
  }

  void dump(const char *mes = "") {
    fprintf(stderr,"%12ld %12ld %s\n",sec(), usec(), mes);
  }
  
};



#endif /* _timeval_h_ */
