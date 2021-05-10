## pthread_mutex_t
bu bir nevi sadece arkadan kilitlenebilen tuvalet kapısı ve sırasıdır.
Giren kişi, pthread_mutex_lock diyerek kapıyı arkadan kilitler,  
başka birisi pthread_mutex_lock derse içerideki "hop kardeşim, dolu" der, bu yüzden lock'layan diğer kişi sıraya geçer.  
İçerideki kişi pthread_mutex_unlock dedigi an siradaki ilk kişi içeri girer ve kapıyı kilitler  


## pthread_cond_t
Bu bir condition içerisinde çağırılması gereken bir şeydir.  
pthread_cond_wait denen fonksiyonu çağırdığı anda fonksiyon içerisine verilen  
cond değişkeninin signal olmasını bekler.  
Yani bir nevi hastane sirasidir. Fonksiyon içerisine bir de mutex alır, bu da wait'in içerisinde çağrıldığı mutex'tir. 
Boş yere tutmaması için alır bunu.  
pthread_cond_wait yapan kişi beklemeye geçer, içerisine cond_t almıştır. 
Diğer taraftan doktor signalledigi zaman siradaki kisi içeri girebilir. mutex'i de alır eline.  
Doktor herkesi birden de çağırabilir, buna da broadcast denir.


###Örnekler:

Doktor odasına giriş için hastalar vardir. Aynı anda bir hasta bulunabilecektir. Bu yüzden gelen hasta kapıyı kilitler.  
Ancak bazı hastaların röntgen çekinmesi gerektiği için onlar odadan çıkarılır ve beklemeye geçer, röntgen bilgileri gelen hasta, doktor tarafından çağırılır signallenir.  
Bunun sonucunda hasta tekrar giris yapar ve iceri gecerek kapiyi kilitler


semaphore'da mutex'e gerek yoktur, cunku direkt beklemez, lock gibidir, ancak cond'da mutex vardir cunku direkt beklemeye geçer, bu yüzden mutex'i salmalıdır, programı dondurmamak icin
