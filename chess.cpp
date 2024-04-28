// g++ -o mywindow chess.cpp `pkg-config gtkmm-3.0 --cflags --libs
#include <gtkmm.h>
#include <array>
#include <iostream>

class Figure;
class Field;


class Cell{	
	int x;
	int y; //coordinate
	
public:
	Figure * point;//указатель на базывай класс фигуры
	bool flag;//необходимо для проверки хода устанавливается в тру если фигура может перейти на это поле
	Cell(bool f, int x,int y){
		this->x=x; 
		this->y=y;
		flag=f;
		point=nullptr;
	}
	Cell(){};
	int get_cor(){return x*y;}
};

class Figure{
public:
	bool side;//True-Белые  False- Черные
	virtual void move(int cur_cor,Field & obj)=0;//функция ничего не возвращает но меняет в ячейках куда можно пойти флаг
	virtual std::string load_path(int cur_cor,Field & obj)=0;//функция возвращает путь до картинки фигуры, координаты нужны чтобы понять на белом или черном фоне должна быть фигура
	virtual bool it_s_king(){//функция проверяет координату и говорит король это или нет. Нужна для проверки игры на завершение
		return 0;}

};

class Field{
	Cell cells[64];
	Cell* last_use;
public:
	Field();
	bool king;// True- если жив False- если мерт
	bool now;//True - ход белых   False - ход черных
	~Field();
	void set_last_use(int i){
		last_use=&cells[i];
	}
	Cell* get_last_use(){
		return last_use;
	}
	Cell& operator[](int i){
	return cells[i];
	}
	void remove_figure(int cor){
		if ((*this)[cor].point!=nullptr &&(*this)[cor].point->it_s_king()){
		std::cout<<"KIng dead"<<std::endl;king =false;now=(*this)[cor].point->side;}
		delete (*this)[cor].point;
		auto figure=last_use->point;
		(*this)[cor].point=figure;
		
		last_use->point=nullptr;
		last_use=nullptr;
		
	}
	void clean_flag(){
		for (int i=0;i<64;i++){
			(*this)[i].flag=false;
		}
	
	}
};

class Pawn: public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
	public:
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		if (side) cur_y--; else cur_y++;
		if (cur_y<0 || cur_y>7) return;
		if (obj[cur_y*8+cur_x].point==nullptr){
		
			obj[cur_y*8+cur_x].flag=true;
			if (side && ++cur_y==6){
				(obj[4*8+cur_x]).flag=true;
				
			}
			if (!side && --cur_y==1){
				(obj[3*8+cur_x]).flag=true;
				
		}}
		cur_x--;
		cur_y=cur_cor/8;
		if (side) cur_y--; else cur_y++;
	
		if (cur_x>=0 && obj[cur_y*8+ cur_x].point!=nullptr && obj[cur_y*8+ cur_x].point->side!=my_side ){
			obj[cur_y*8+cur_x].flag=true;
			
		}
		cur_x+=2;
		if (cur_x<=7 && obj[cur_y*8+ cur_x].point!=nullptr && obj[cur_y*8+ cur_x].point->side!=my_side ){
			obj[cur_y*8+cur_x].flag=true;
		}
	}
	virtual std::string load_path(int cur_cor,Field & obj){
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	
	Pawn(bool flag){side=flag;}
	Pawn(){};

};

bool help_move(int cur_y,int cur_x,Field & obj,bool my_side){
	
	if (obj[cur_y*8+cur_x].point==nullptr){
		obj[cur_y*8+cur_x].flag=true;
		}
		else {
				if(obj[cur_y*8+cur_x].point->side!=my_side){
					obj[cur_y*8+cur_x].flag=true;
				}
			    return 0;}
	return 1;
};



class Rook:public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
	public:
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		while (++cur_x<=7){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_x=cur_cor%8;
		while (--cur_x>=0){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_x=cur_cor%8;
		while (--cur_y>=0){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		while (++cur_y<=7){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
	
	
	
	}
	std::string load_path(int cur_cor,Field & obj){//да функция полностью повторяется во всех классах наследниках, однако я не знаю как сделать так чтобы она не повторялась я пытался(
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	Rook(){};
};



class Bishop:public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
	public:
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		while (++cur_x<=7 && ++cur_y<=7){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (--cur_x>=0 && ++cur_y<=7){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (--cur_x>=0 && --cur_y>=0){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (++cur_x<=7 && --cur_y>=0){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
}
	std::string load_path(int cur_cor,Field & obj){//да функция полностью повторяется во всех классах наследниках, однако я не знаю как сделать так чтобы она не повторялась я пытался(
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	Bishop(){};



};

class Queen:public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
public:
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		while (++cur_x<=7 && ++cur_y<=7){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (--cur_x>=0 && ++cur_y<=7){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (--cur_x>=0 && --cur_y>=0){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (++cur_x<=7 && --cur_y>=0){if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		while (++cur_x<=7){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_x=cur_cor%8;
		while (--cur_x>=0){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_x=cur_cor%8;
		while (--cur_y>=0){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
		cur_y=cur_cor/8;
		while (++cur_y<=7){	if (!::help_move(cur_y,cur_x,obj,my_side)) break;}
	
	
	}
	std::string load_path(int cur_cor,Field & obj){//да функция полностью повторяется во всех классах наследниках, однако я не знаю как сделать так чтобы она не повторялась я пытался(
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	Queen(){};


};

class King:public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
public:
	bool it_s_king(){return 1;}
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		if (++cur_x<=7 && ++cur_y<=7){::help_move(cur_y,cur_x,obj,my_side);}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		if (--cur_x>=0 && ++cur_y<=7){::help_move(cur_y,cur_x,obj,my_side);}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		if (--cur_x>=0 && --cur_y>=0){::help_move(cur_y,cur_x,obj,my_side);}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		if (++cur_x<=7 && --cur_y>=0){::help_move(cur_y,cur_x,obj,my_side);}
		cur_y=cur_cor/8;
		 cur_x=cur_cor%8;
		if (++cur_x<=7){::help_move(cur_y,cur_x,obj,my_side);}
		cur_x=cur_cor%8;
		if (--cur_x>=0){::help_move(cur_y,cur_x,obj,my_side);}
		cur_x=cur_cor%8;
		if (--cur_y>=0){::help_move(cur_y,cur_x,obj,my_side);}
		cur_y=cur_cor/8;
		if (++cur_y<=7){::help_move(cur_y,cur_x,obj,my_side);}
	
	}
	std::string load_path(int cur_cor,Field & obj){//да функция полностью повторяется во всех классах наследниках, однако я не знаю как сделать так чтобы она не повторялась я пытался(
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	King(){};
};



class Knight:public Figure{
	static std::string path1;
	static std::string path2;
	static std::string path3;
	static std::string path4;
	static std::string path5;
	static std::string path6;
public:
	void move(int cur_cor,Field & obj){
		int cur_y=cur_cor/8;
		int cur_x=cur_cor%8;
		bool my_side=obj[cur_y*8+ cur_x].point->side;
		cur_x-=2;
		if (cur_x>=0){
			if (--cur_y>=0){help_move(cur_y,cur_x,obj,my_side);}
			cur_y++;
			if(++cur_y<=7){help_move(cur_y,cur_x,obj,my_side);}
		}
		cur_y=cur_cor/8;
		cur_x=cur_cor%8;
		cur_x+=2;
		if (cur_x<=7){
			if (--cur_y>=0){help_move(cur_y,cur_x,obj,my_side);}
			cur_y++;
			if(++cur_y<=7){help_move(cur_y,cur_x,obj,my_side);}
		}
		cur_y=cur_cor/8;
		cur_x=cur_cor%8;
		cur_y+=2;
		if (cur_y<=7){
			if (--cur_x>=0){help_move(cur_y,cur_x,obj,my_side);}
			cur_x++;
			if(++cur_x<=7){help_move(cur_y,cur_x,obj,my_side);}
		}
		cur_y=cur_cor/8;
		cur_x=cur_cor%8;
		cur_y-=2;
		if (cur_y>=0){
			if (--cur_x>=0){help_move(cur_y,cur_x,obj,my_side);}
			cur_x++;
			if(++cur_x<=7){help_move(cur_y,cur_x,obj,my_side);}
		}
	};
	std::string load_path(int cur_cor,Field & obj){//да функция полностью повторяется во всех классах наследниках, однако я не знаю как сделать так чтобы она не повторялась я пытался(
		if (side){
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path3;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path2;
			}
			else 
				return path1;
		}
		else{
			if (obj[cur_cor].flag){//если флаг выставлен значит на это поле может быть ход -> подсвечиваем его
				return path6;
			}
			if ((cur_cor/8+cur_cor%8)%2){
				return path5;
			}
			else 
				return path4;
		}
	}
	Knight(){};
};

Field::Field(){
		for (int i = 0; i < 8; i++)
		{
		   for (int j = 0; j < 8; j++)
			{	
			  cells[i*8+j]=Cell(false,i,j);
			  if (i==0){
					switch (j) {
  
					  case 0:
					  case 7:
						cells[i*8+j].point=new Rook;
						break;

					  // Если i равно 1 или 6, печатаем 2
					  case 1:
					  case 6:
						cells[i*8+j].point=new Knight;
						break;

					  // Если i равно 2 или 5, печатаем 3
					  case 2:
					  case 5:
						cells[i*8+j].point=new Bishop;
						break;

					  // Если i равно 3, печатаем 4
					  case 3:
						cells[i*8+j].point=new Queen;
						break;

					  // Если i равно 4, печатаем 5
					  case 4:
						cells[i*8+j].point=new King;
						break;}
					cells[i*8+j].point->side=false;

			  }
			  if (i==1){
			  		cells[i*8+j].point=new Pawn;
			  		cells[i*8+j].point->side=false;
			  }
			  if (i==6){
			 	 cells[i*8+j].point=new Pawn;
			 	 cells[i*8+j].point->side=true;
			  }
			  if (i==7){
			  	
					switch (j) {
  
					  case 0:
					  case 7:
						cells[i*8+j].point=new Rook;
						break;

					  // Если i равно 1 или 6, печатаем 2
					  case 1:
					  case 6:
						cells[i*8+j].point=new Knight;
						break;

					  // Если i равно 2 или 5, печатаем 3
					  case 2:
					  case 5:
						cells[i*8+j].point=new Bishop;
						break;

					  // Если i равно 3, печатаем 4
					  case 3:
						cells[i*8+j].point=new King;
						break;

					  // Если i равно 4, печатаем 5
					  case 4:
						cells[i*8+j].point=new Queen;
						break;}
					cells[i*8+j].point->side=true;
			  }
			}
		}
		last_use=nullptr;
		now=true;
		king=true;
		
}
Field::~Field(){
	for(int i=0;i<64;i++){
	delete cells[i].point;	
	}
}

class SecondWindow : public Gtk::Window {
	Gtk::Label label;
  public:
    // Конструктор класса
    SecondWindow(bool now){  // Устанавливаем размер окна
  set_default_size(200, 200);

  // Устанавливаем заголовок окна
  set_title("Игра завершена");
  if (now)
   label.set_text("Игра завершена, белые выйграли ");
   else
   	label.set_text("Игра завершена, черные выйграли ");

  // Добавляем надпись в окно
  add(label);
  show_all();
};

    // Деструктор класса
    ~SecondWindow(){};
};


class MyWindow : public Gtk::Window
{
public:
  MyWindow(Field * a);
  virtual ~MyWindow();
  void on_second_window_closed();

protected:
  // Обработчик сигнала
  void on_button_clicked(int row, int col);
  void load_image(int row, int col, std::string path);
  void updating_the_screen();
   SecondWindow* second_window_;
  //Поле 
  Field* a;
  // Виджеты
  Gtk::Grid m_grid;
  std::array<std::array<Gtk::Button, 8>, 8> m_buttons; // Массив из 64 кнопок
};

// Конструктор
MyWindow::MyWindow(Field * a)
{
	this->a=a;
  // Устанавливаем заголовок и размер окна
  set_title("Шахматная доска");
  set_default_size(640, 640);

  // Цикл по строкам и столбцам сетки
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      // Создаем кнопку
      m_buttons[i][j] = Gtk::Button();

      // Соединяем кнопку с обработчиком сигнала, передавая ей координаты
      m_buttons[i][j].signal_clicked().connect(sigc::bind<int, int>(
        sigc::mem_fun(*this, &MyWindow::on_button_clicked), i, j));

      // Добавляем кнопку в сетку
      m_grid.attach(m_buttons[i][j], j, i);
      
      m_buttons[i][j].set_hexpand(true);
      m_buttons[i][j].set_vexpand(true);
      
    }
  }
  for (int i=0;i<64;i++){
  	std::string path;
  	if ((*a)[i].point!=nullptr){
  		std::string path=((*a)[i].point)->load_path(i,*a);
  		load_image(i/8,i%8,path);
  	}
  	else{
  		if ((i/8+i%8)%2){
  			std::string path="./image/black.jpg";
  			load_image(i/8,i%8,path);}
  			
  		else{
  			std::string path="./image/white.jpg";
  			load_image(i/8,i%8,path);}
  	}
  	
  	
  
  }
 
  //load_image(0,0,"../image/0w.png");
  // Добавляем сетку в окно с помощью функции add()
  add(m_grid);
  show_all();
  second_window_ = nullptr;
}
void MyWindow::load_image(int row, int col, std::string path)
{
  // Создаем объект Gtk::Image с помощью конструктора, который принимает путь к файлу изображения и размер изображения
  Gtk::Image* image = new Gtk::Image(path);
  /./image->set_size_request(79,79);
  // Вызываем метод set_image у кнопки, которая соответствует координатам row и col, и передаем ей объект Gtk::Image в качестве аргумента
  m_buttons[row][col].set_image(*image);
}
// Деструктор
MyWindow::~MyWindow(){}
void MyWindow::updating_the_screen()
{
	for (int i=0;i<64;i++){
  	if ((*a)[i].point!=nullptr){
  		std::string path=((*a)[i].point)->load_path(i,*a);
  		load_image(i/8,i%8,path);
  	}
  	else{
  		if ((i/8+i%8)%2){
  			std::string path="./image/black.jpg";
  			load_image(i/8,i%8,path);}
  		else{
  			std::string path="./image/white.jpg";
  			load_image(i/8,i%8,path);}
  		if ((*a)[i].flag){
  			std::string path="./image/green.png";
  			load_image(i/8,i%8,path);}
  	}
}
}


void MyWindow::on_second_window_closed() {
  // Удаляем экземпляр второго окна
  delete second_window_;

  // Обнуляем указатель на второе окно
  second_window_ = nullptr;
  this->close();

};
// Обработчик сигнала
void MyWindow::on_button_clicked(int row, int col)
{
	if (!((*a).king)){
	
			if (second_window_ != nullptr) return;

  // Создаем новый экземпляр второго окна
  second_window_ = new SecondWindow((*a).now);

  // Подключаем обработчик закрытия второго окна
  second_window_->signal_hide().connect(sigc::mem_fun(*this, &MyWindow::on_second_window_closed));

  // Показываем второе окно
  second_window_->show();
	
	}
  if ((((*a)[row*8+col].point!=nullptr) &&  ((*a)[row*8+col].point->side==(*a).now)) ||((*a)[row*8+col].point==nullptr)||((*a)[row*8+col].flag)){
  // Выводим сообщение в консоль
  if ((*a)[row*8+col].flag){
  		//уничтожение фигуры и перстановка на ее место
  		
  		a->remove_figure(row*8+col);
  		a->clean_flag();
  		(*a).now=!(*a).now;
  }
  else{
  	  a->clean_flag();
	  //std::cout << "Нажата кнопка в строке " << row << " и столбце " << col << std::endl;
	  if ((*a)[row*8+col].point!=nullptr){
	  	(*a)[row*8+col].point->move(row*8+col,*a);
	  	a->set_last_use(row*8+col);

	  }
	}}
  updating_the_screen();
  
}






std::string Rook::path1="./image/15w.png";
std::string Rook::path2="./image/15b.png";
std::string Rook::path3="./image/15g.png";
std::string Rook::path4="./image/5w.png";
std::string Rook::path5="./image/5b.png";
std::string Rook::path6="./image/5g.png";

std::string Pawn::path1="./image/10w.png";
std::string Pawn::path2="./image/10b.png";
std::string Pawn::path3="./image/10g.png";
std::string Pawn::path4="./image/0w.png";
std::string Pawn::path5="./image/0b.png";
std::string Pawn::path6="./image/0g.png";

std::string Bishop::path1="./image/13w.png";
std::string Bishop::path2="./image/13b.png";
std::string Bishop::path3="./image/13g.png";
std::string Bishop::path4="./image/3w.png";
std::string Bishop::path5="./image/3b.png";
std::string Bishop::path6="./image/3g.png";

std::string Queen::path1="./image/12w.png";
std::string Queen::path2="./image/12b.png";
std::string Queen::path3="./image/12g.png";
std::string Queen::path4="./image/2w.png";
std::string Queen::path5="./image/2b.png";
std::string Queen::path6="./image/2g.png";

std::string King::path1="./image/11w.png";
std::string King::path2="./image/11b.png";
std::string King::path3="./image/11g.png";
std::string King::path4="./image/1w.png";
std::string King::path5="./image/1b.png";
std::string King::path6="./image/1g.png";

std::string Knight::path1="./image/14w.png";
std::string Knight::path2="./image/14b.png";
std::string Knight::path3="./image/14g.png";
std::string Knight::path4="./image/4w.png";
std::string Knight::path5="./image/4b.png";
std::string Knight::path6="./image/4g.png";

// Главная функция
int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
  Field a;
  
  // Создаем объект нашего класса окна
  MyWindow window(&a);

  // Показываем окно и запускаем приложение
  return app->run(window);
}

