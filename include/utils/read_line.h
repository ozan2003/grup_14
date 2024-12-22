/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */
#ifndef READ_LINE_H
#define READ_LINE_H

#define BUFFER_SIZE 256

/**
 * @brief Bir satır okur ve döndürür.
 * @return Okunan satır.
 *
 * @warning Bu fonksiyon, bellek yönetimi için malloc ve realloc fonksiyonlarını
 * kullanır. Bu nedenle, kullanılan belleği serbest bırakmak için free
 * fonksiyonunu kullanmayı unutmayın.
 */
char* read_line(void);

#endif // READ_LINE_H