#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <locale.h>
#include "ConsoleColor.h"
#include <time.h>

using namespace std;

struct node {
	node* next;		//sonraki eleman
	node* prev;		//önceki eleman
	int data;		//kelimenin metin içinde bulunduğu indis tutulacak.
	int sira;		//kelimenin kaçıncı bulunan kelime olduğunun bilgisi tutulacak.
};

clock_t Tbasla, Tbitir, Ybasla, Ybitir;
int sayac = 0, tekrar = 0, mesafe = 100, geciciMesafe = 0;;
char c;
bool kelimeTekmi = false;  //kelimenin metin içinde tek başına olup olmadığının (sağında ve solunda ek harf yoksa)  bilgisi tutulacak
string aranan = "";
string benzerKelime;
string metin = "";


string KelimeKucult(string kelime)  // Gelen kelime küçük harfli hale dönüşür.
{
	for (int i = 0; i < kelime.length(); i++)
	{
		kelime[i] = tolower(kelime[i]);
	}
	return kelime;
}

int** MatrisOlustur(int m, int n)		//kelime uzunlukları program içinde sürekli değişik olacağından dinamik bir matris oluşturduk.
{
	int** Matrix;

	Matrix = new int* [m];  //satır
	for (int i = 0; i < m; i++)   
		Matrix[i] = new int[n];	//sütun

	return Matrix;
}

node* ListeyeEkle(node* r, int x, int s) {
	if (r == NULL)				//liste boşsa root'un göstereceği ilk node u oluşturup oraya ekleme yaptık.
	{
		r = new node;
		r->data = x;
		r->sira = s;
		r->next = r;
		r->prev = r;
	}
	else {						//lsitede eleman varsa root'un soluna yani listenin sonuna ekleme yaptık.
		node* tmp;
		tmp = new node;
		tmp->data = x;
		tmp->sira = s;
		r->prev->next = tmp;
		tmp->next = r;
		tmp->prev = r->prev;
		r->prev = tmp;
	}
	return r;
}

void YalnizKelimemi(char SolHarf, char SagHarf)		//aranan kelimenin solundaki ve sağındaki karakterlerin indislerinini aldık. 
{
	if (!((65 <= SolHarf && SolHarf <= 90) || (97 <= SolHarf && SolHarf <= 122)))   //soldaki harf büyük ya da küçük harf değilse (' ','.',':' vs.) solunda eki yoktur. cümle sonu vs. olabileceği için '.'gibi karakterler olmasında sıkıntı yok
	{
		if (!((65 <= SagHarf && SagHarf <= 90) || (97 <= SagHarf && SagHarf <= 122)))  //sağındaki harf büyük ya da küçük harf değilse kelime tektir .
		{																							//(sağında ve solunda harf yoktur)
			kelimeTekmi = true;
		}
	}
}


int TamEslesme(string p, string t, int i) {  //i =metin içinde arama yapmaya başladığımız indis.
	int m = p.length();     // m = aradığım kelimenin uzunluğu
	int n = t.length();		// n = Arama yapacağım metnin uzunluğu
	int j = 0;				// j = Eşleşen harf sayısı

	while (i < t.length())   //i = metnin uzunluğuysa zaten metnin son harfindeyiz orada bir kelime bulamayız yani işlem bitmiştir, metnin sonuna kadar aradık while'a girmemize artık gerek yok 
	{
		if (tolower(t[i]) == tolower(p[j]))  //arananın harfiyle metinden aldığım harfleri küçültüp karşılaştırıyoruz.
		{
			if (j == p.length() - 1)	//bu harfler eşit ama arananın hangi indisi idi? eğer eşit olan bu harfler son indislerseler aradığımız kelimeyi bulmuşuz demektir
			{
				if ((i - (int)p.length()) >= 0 && (i + 1) < t.length())		//bulduğumuz kelime sonda ve başta değilse 
				{																	//solundaki ve sağındaki harflerin indislerini harf mi? diye kontrol etmek için kontrole gönderiyoruz
					YalnizKelimemi(t[i - (int)p.length()], t[i + 1]);	// t[i - p.length()] = kelimenin Solu    || t[i + 1] = kelimenin Sağı
				}
				return i - j;		//kelimenin metindeki başladığı indis.
			}
			else			//bu harfler eşit ama aradığımız kelimenin sonundaki indisi henüz karşılaştırmadıysak işlem bitmemiştir
			{					// sonraki harfleri karşılaştırmak için i'yi ve j'yi 1 artırıyoruz.
				i += 1;
				j += 1;
			}
		}
		else       //karşılaştırdığımız harfler eşit değilseler bazı kontroller yapacağız.
		{
			if (j > 0)    //j>0 yani eşit olmayan harfler ortada biryerde. bu yüzden j'yi tekrar baştan yani 0 dan başlatıyoruz.
			{					//i'yi arttırmıyoruz çünkü kelimenin ilk harfi i'deki harfle aynı olabilir
				j = 0;
			}
			else  //j=0   yani ilk harfler eşit değil bu yüzden j aranan kelimenin indisini gösteriyor.zaten 0 olduğı için buna dokunmamıza gerek yok.
			{
				i += 1;		//metindeki eşit olmayan harfin indisi yani i'yi 1 artırıp metinde gezmeye devam ediyoruz
			}
		}
	}
	return -1000; //bu değer dönerse kelime metinde yok demektir.
}


pair<node*, int> KelimeleriListele(node* r, string kelime, string metinParcasi, bool Listelensinmi)
{
	sayac = 0;

	if (r != NULL)
		r = NULL;

	int BulunanIndis = kelime.length() * -1;

	if (Listelensinmi)
		Tbasla = clock();
	while (BulunanIndis != -1000)
	{
		BulunanIndis = TamEslesme(kelime, metinParcasi, BulunanIndis + kelime.length());

		if (BulunanIndis != -1000)
		{
			sayac++;

			if (Listelensinmi)
				r = ListeyeEkle(r, BulunanIndis, sayac);
		}
	}
	if (Listelensinmi)
		Tbitir = clock();

	return make_pair(r, sayac);
}


int Minimum(int a, int b, int c) { //gelen a, b, c den en küçük olanın hangisi olduğunu geri döndüren fonksiyon
	if (a <= b && a <= c)
		return a;
	if (b <= c && b <= a)
		return b;
	if (c <= a && c <= b)
		return c;
}

int Levenshtein(string k, string o) {

	int m = k.length();			//kelimenin boyutu m
	int n = o.length();			//okunanın boyutu n

	int** d = MatrisOlustur(m + 1, n + 1);   //mxn dinamik matris oluşturulur.

	for (int i = 0; i < m + 1; i++)			//matrisin ilk sütununa sırasıyla 0'dan m e kadar değerler atanır.
		d[i][0] = i;
	for (int i = 0; i < n + 1; i++)			//matrisin ilk satır sırasıyla 0'dan n e kadar değerler atanır.
		d[0][i] = i;


	for (int i = 1; i <= m; i++)		//d matrisinin 1. satırından başlayarak son satırına kadar gezme işlemi.
	{
		for (int j = 1; j <= n; j++)	//d matrisinin 1. sütunundan başlayarak son sütununa kadar gezme işlemi.
		{

			if (k[i - 1] == o[j - 1])	//1. kelimenin i-1. indisindeki karakterle 2.kelimenin j-1. indindeki karaktere eşitse.
			{
				d[i][j] = d[i - 1][j - 1];		//sol üst çaprazdaki değer d[i][j]'ye yazılır.
			}
			else
			{
				d[i][j] = Minimum(					//eğer karkterler eşit değilse  solu, üstü ve sol üstüne ait indislerdeki değerlerin 1 eklenmiş hallerinden en küçüğü d[i][j]'ye yazılır. 
					d[i - 1][j] + 1,  // eğer üst taraftan almışsak kelime silerek benzetme yapılır.
					d[i][j - 1] + 1,  // eğer sol taraftan almışsak kelime ekleyerek benzetme yapılır.	
					d[i - 1][j - 1] + 1 // eğer sol üst taraftan almışsak kelimeler yer değiştirilerek benzetme işlemi yapılır.
				);
			}
		}
	}
	return d[m][n];		//d dizisinin son elemanı olan değer bize iki kelime arasındaki farkı verir ve bunu geri döndürüyoruz.
}


void NodeIcindeGez(node* r, string kelime)  //did you mean fonksiyonundan çektiğimiz mesafesi küçük olan benzer kelimeler metinden daha önce den listelediğimiz indislerin yakınlarında tekrar aranır.
{
	kelimeTekmi = false;		 //benzer kelimelerin arasından anlamlı olanı bulmak istediğimiz için her arama işleminden önce kelimTekmi'yi sıfırlıyoruz. Çünkü daha önceden aranan benzer kelimede true olmuş olabilir.
	string yeniMetin = "";      //ilk aradığımız kelimenin metindeki bulunduğu kısmı küçük parçaya ayırarak yeni metine aktarırız ve aramalarımızı burada gerçekleştiririz.
	int geciciTekrar = 0;		//en çok tekrar eden benzer kelimeyi bulmak istediğimiz için yaptığımız bir kontrol değişkeni
	int aciklik = 3;			//metinden parçalayacağım kısmın büyüklüğü

	if (r->data < aciklik)    //kelimenin basladiği indis açıklıktan küçükse, açıklık metnin '-' indislerinde harf arayacağı için açıklığı küçültmemiz gerekiyor
		aciklik = r->data;

	if (metin.length() - r->data - kelime.length() < aciklik) //kelimenin sağındaki harflerin sayısı acıklıktan küçükse olmayan indislerde harf aranacağı için açıklık küçültüldü
		aciklik = metin.length() - r->data - kelime.length();

	node* yrd = r;
	do
	{
		yeniMetin = metin.substr(yrd->data - aciklik, kelime.length() + (2 * aciklik)); //ilk aradığımız kelimenin metindeki bulunduğu kısmı küçük parçaya ayırıyoruz.
		geciciTekrar += KelimeleriListele(NULL, kelime, yeniMetin, false).second;       //bu parçalarda yaptığımız tam eşleşmelerden kelimenin kaç kere tekrar ettiğini buluyoruz

		yrd = yrd->next;
	} while (yrd != r); //Bağlı listede gezme işlemi

	if (tekrar <= geciciTekrar && mesafe >= geciciMesafe && kelimeTekmi) //Burada en çok tek tekrar eden, mesafesi en küçük ve yalnız başına olan kelimeyi benzerKelime değişkenine atıyoruz.
	{
		mesafe = geciciMesafe; 
		tekrar = geciciTekrar;
		benzerKelime = kelime;
	}

}

bool DidYouMean(node* r, string kelime, bool hicVarmi)
{
	benzerKelime = "-";

	Ybasla = clock();
	for (int i = -2; i <= 2; i++)
	{
		if (kelime.length() + i > 1)   // Aradığım karakter textleri en az 2 karakterli olmalıdır.
		{
			string isim = to_string(kelime.length() + i) + "_karakter.txt";
			string okunan;

			ifstream dosyaoku(isim);
			while (!dosyaoku.eof())
			{
				getline(dosyaoku, okunan);
				geciciMesafe = Levenshtein(kelime, okunan);
				if (geciciMesafe < 4 && kelime != okunan)
				{
					if (hicVarmi)
					{
						NodeIcindeGez(r, okunan);
					}
					else
					{
						if (mesafe >= geciciMesafe)
						{
							mesafe = geciciMesafe;
							benzerKelime = okunan;
						}
					}
				}
			}
			dosyaoku.close();
		}
	}
	Ybitir = clock();

	if (benzerKelime != "-")
	{
		cout << "Bunu mu demek istediniz? = " << benzerKelime << endl << " e : Evet " << endl << " h : Hayır " << endl;
		cout << yellow << "Yakın eşleşme hızı: " << (float)(Ybitir - Ybasla) / CLOCKS_PER_SEC << "sn" << white << endl;

		switch (tolower(_getch()))
		{
		case'e':
			return true;
			break;
		}
	}

	return false;
}


void DosyadanOku(string dosya)
{
	ifstream dosyaoku(dosya);
	string okunan;
	while (!dosyaoku.eof())		//okunmak istenen dosyayı sonuna gelene kadar okuyup
	{
		getline(dosyaoku, okunan);		//her okunan satırı önceki okuduğumun sonun ekliyorum. böylece arama yapacağım metni dosyadan çekmiş oldum.
		metin += okunan + " ";
	}
	dosyaoku.close();
}


void Goster(node* r)
{								//(root'un datası aradığımız kelimenin ilk harfinin metindeki bulunduğu indis idi.)
	int aciklik = 275;     //kelimenin bulunduğu indisin sağ ve solundaki belli miktar harfi de yazdırıp aranan kelimenin hangi cümlede  geçtiğini de görebilmek için açıklık tanımladık.

	if (r->data < aciklik)    //bulunan indis acıklıktan küçükse açıklık metnin '-' indislerinde harf arayacağı için açıklığı küçültmemiz gerekiyor
		aciklik = r->data;

	cout << metin.substr(r->data - aciklik, aciklik);  //kelimenin sol tarafındaki acıklık kadar harf ekrana yazıldı.

	cout << red << metin.substr(r->data, aranan.length()) << white;   //kelime kırmızı olarak ekrana yazıldı.

	if (metin.length() - r->data - aranan.length() < aciklik)  //kelimenin sağındaki harflerin sayısı acıklıktan küçükse olmayan indislerde harf aranacağı için açıklık küçültüldü
		aciklik = metin.length() - r->data - aranan.length();

	cout << metin.substr((r->data) + aranan.length(), aciklik);   //kelimenin sağındaki açıklık kadar harf ekrana yazıldı

	cout << "\n\n\n" << green << r->sira << white << " / " << green << sayac << white << endl;  //gez fonksiyonundayken sayaca kaç tane kelime bulduğumuzu gödermiştik
}																							//ekrana "bulunduğum sıra / toplam kelime sayısı" şeklinde yazı yazdırdık.

void Gez(node* r)
{
	sayac = r->prev->sira;		//son elemanın sırası listede kaç tane bulunmuş kelime olduğunu gösteriyor.
	do {						//ilk başta root taki veriye göre bulunmuş kelime konsolda gösterilecek daha sonra kullanıcı ok tuşlarıyla bulunmuş kelimeler arasında gezebilecek
		system("cls");
		Goster(r);			//roottaki indisin gösterilmesi için göster'e yollanır.	
		cout << yellow << "Tam eşleşme hızı: " << (float)(Tbitir - Tbasla) / CLOCKS_PER_SEC << "sn" << white << endl;

		c = _getch();
		switch (c)
		{
		case 77:   //sağ ok tuşuna basıldığında root sağa kaydırılır.
			r = r->next;
			break;
		case 75:   //sol ok tuşuna basıldığında root sola kaydırılır.
			r = r->prev;
			break;
		}
	} while (c != 'x');  //x e basıldığında gezinme sonlandırılır.
}

void dosyasecimi() { //aramanın hangi dosyada yapılacağını seçme.
	system("cls");
	char ds;
	cout << "1- Crime _and_Punishment.xml\n";
	cout << "2- Gullivers_Travel.html\n";
	cout << "3- Robinson_Crusoe.txt             Hangi dosyada arama yapmak istersiniz ?\n";
	cout << "4- The_Adventures.docx\n";
	cout << "5- Tüm Dosyalar                    "; ds = _getch();

	switch (ds)
	{
	case '1':
		DosyadanOku("Crime _and_Punishment.xml");
		break;
	case '2':
		DosyadanOku("Gullivers_Travel.html");
		break;
	case '3':
		DosyadanOku("Robinson_Crusoe.txt");
		break;
	case '4':
		DosyadanOku("The_Adventures.docx");
		break;
	case '5':
		DosyadanOku("Crime _and_Punishment.xml");
		DosyadanOku("Gullivers_Travel.html");
		DosyadanOku("Robinson_Crusoe.txt");
		DosyadanOku("The_Adventures.docx");
		break;
	default:
		dosyasecimi();
		break;
	}
	system("cls");
}

int main(void) {
	setlocale(LC_ALL, "Turkish");
	node* root;
	root = NULL;

	dosyasecimi();  //aramanın hangi dosyada yapılacağını seçme.

	cout << "Aranacak Kelimeyi Giriniz  : ";
	getline(cin, aranan);
	aranan = KelimeKucult(aranan);  //alınan kelimeyi küçük harfe çevirme


	root = KelimeleriListele(root, aranan, metin, true).first;   //aranan kelimenin metin içindeki başlangıç indisleri listeye aktarılır ve bu listenin başlangıcı root'a döndürülür.

	if (root != NULL)			// Kelime Bulunduysa
	{
		if (kelimeTekmi)		// Kelime tek başına bulunduysa (sağı ve solunda ek harafleri yoksa) gösterilir.
			Gez(root);
		else					// Kelime tek başına bulunamadıysa (sağı ve solunda ek harafleri varsa) did you mean işlemi yapılır.
		{
			if (DidYouMean(root, aranan, true))  //kullanıcı did you mean? sorusana evet derse önerilen kelime metinde aranır.
			{
				aranan = benzerKelime;
				root = KelimeleriListele(root, aranan, metin, true).first; //önerilmiş kelime metinde aranıyor.

				if (root != NULL)  //önerilmiş kelime metinde varsa listeye eklenmiştir bu yüzden root NULL değildir. gösterebiliriz
					Gez(root);
				else               //root NULL ise önerilmiş kelime de bulunamamıştır.
					cout << "Kelime bulunamadı.";
			}
			else                                //kullanıcı did you mean? sorusana hayır derse yazdığı kelimenin tek başına olmadığı yerler gösterilir
			{
				Gez(root);
			}
		}
	}
	else						 // Kelime Bulunamadıysa did you mean? işlemi yapılır.
	{
		if (DidYouMean(NULL, aranan, false))  ///kullanıcı did you mean? sorusana evet derse önerilen kelime metinde aranır.
		{
			aranan = benzerKelime;
			root = KelimeleriListele(root, aranan, metin, true).first;   //önerilmiş kelime metinde aranıyor.

			if (root != NULL)		//önerilmiş kelime metinde varsa listeye eklenmiştir bu yüzden root NULL değildir. gösterebiliriz
				Gez(root);
			else                   //root NULL ise önerilmiş kelime de bulunamamıştır.
				cout << "Kelime bulunamadı.";
		}
		else                       //aranan kelime metinde hiç olmadığı için, kullanıcı önerilen kelimeyi aramak istemezse aradığının olmadığını döndürüyoruz.
			cout << "Kelime bulunamadı.";
	}

	return 0;
}
