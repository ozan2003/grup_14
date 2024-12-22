# Grup üyeleri:
# - Eren Çoban - G221210090
# - Onur Eğrikılıç - G221210066
# - Ozan Malcı - B211210039
# - Semih Öztürk - G221210003
# - Yiğit Talha Adagülü - G221210093

CC := gcc
CCFLAGS := -g -std=c99 -Wall -Wextra -Wshadow -pedantic
# Bu klasörlerin mevcut olması gerekiyor.
SRCDIR := ./src
INCDIR := ./include
BINDIR := ./bin
LIBDIR := ./lib
# Silme işlemi için kullanılacak komut.
RM := rm
# Hedef çalıştırılabilir dosya adını özelleştirebilirsiniz.
TARGET_EXEC := main

# wildcard fonksiyonu, belirtilen dosya adı desenlerine uygun mevcut dosyaların
# isimlerinden oluşan, boşluklarla ayrılmış bir liste döndürür.
#
# SOURCES bizim .c dosyalarımız olacak.
#
# * dosya sisteminizde uygun dosya adlarını arar.
SOURCES := $(wildcard $(SRCDIR)/*.c)

# $(patsubst pattern,replacement,text)
# patsubst fonksiyonu, bir desenle eşleşen bir stringin her bir alt dizisini başka bir stringle değiştirir.
# % joker karakter gibi çalışır. Genellikle kural tanımlarında ve bazı özel fonksiyonlarda kullanılır.
#
# OBJECTS, SOURCES içerisindeki dosyalarla (file.c -> file.o) eşleşen .o dosyalarımız olacak.
#
# Bir kuraldaki bir % önek/yapı olarak, hedefteki % ile eşleşen aynı gövde (dosya adı) anlamına gelir.
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(LIBDIR)/%.o,$(SOURCES))

# -I her bir bireysel .hpp dosyasının önüne eklenir
# addprefix fonksiyonu ile.
INC_PARAMS := $(addprefix -I,$(INCDIR))

# $@ hedefin adını temsil eder.
# $^ bağımlılıkların adlarını boşluklarla ayrılmış olarak temsil eder.
# $< bağımlılıkların ilk adını temsil eder.
#
# Her .o dosyasını, ona karşılık gelen .c dosyasıyla derlemek için bir desen kuralı tanımlayın.
$(LIBDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) $(INC_PARAMS) -c $< -o $@

# Tüm .o dosyalarını birleştir.
$(BINDIR)/$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o $@

# Phony hedefler, bir dosya oluşturmak için kullanılmıyorsa kullanılır. 
# Make'e bu hedeflerin fiziksel dosyaları temsil etmediğini açıkça belirtmemize olanak tanır.
.PHONY: all
all: $(BINDIR)/$(TARGET_EXEC)

.PHONY: run
run: $(BINDIR)/$(TARGET_EXEC)
	$(BINDIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	$(RM) $(BINDIR)/$(TARGET_EXEC) $(LIBDIR)/*.o
