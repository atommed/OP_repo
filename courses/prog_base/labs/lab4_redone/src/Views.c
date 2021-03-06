#include "Views.h"

#include "QueryingUtil.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static void assign_text(char **dest, const char *src) {
  if (src == NULL)
    *dest = NULL;
  else {
    *dest = malloc(strlen(src) + 1);
    strcpy(*dest, src);
  }
}

void print_user_v(UserV *u) {
  printf("#%lld %s\n------------------\n", u->id, u->name);
}

void init_in_message_v(InMessageV *msg) {
  msg->attachments = NULL;
  msg->body = NULL;
  msg->sender_name = NULL;
}

void finalize_in_message_v(InMessageV *msg) {
  if (msg == NULL)
    return;
  free(msg->attachments);
  free(msg->body);
  free(msg->sender_name);
}

InMessageV *new_in_message_v(void) {
  InMessageV *ret = malloc(sizeof(InMessageV));
  init_in_message_v(ret);
  return ret;
}

void ctr_in_message_v(InMessageV *msg, _id id, _id sender, time_t post_date,
                      char *body, char *attachments, char *sender_name) {
  msg->id = id;
  msg->sender = sender;
  msg->post_date = post_date;

  assign_text(&msg->body, body);
  assign_text(&msg->attachments, attachments);
  assign_text(&msg->sender_name, sender_name);
}

void delete_in_message_v(InMessageV *o) {
  finalize_in_message_v(o);
  free(o);
}

int receive_messages(sqlite3 *db, _id receiver, InMessageV *res, int limit) {
  int received = 0;
  def_stmt("select "
           "s.name, m.id, sender_id ,post_date,body, attachments "
           "from Message m "
           "left join User s on sender_id = s.id "
           "where receiver_id = @receiver "
           "order by post_date limit @limit");

  bind_id_v("@receiver", receiver);
  bind_int_v("@limit", limit);

  while (sqlite3_step(q) == SQLITE_ROW)
    ctr_in_message_v(&res[received++], col_id(1), col_id(2), col_int64(3),
                     col_text(4), col_text(5), col_text(0));

  sqlite3_reset(q);
  return received;
}

void init_post_v(PostV *post) {
  post->attachments = NULL;
  post->body = NULL;
  post->author_name = NULL;
}

void finalize_post_v(PostV *post) {
  if (post == NULL)
    return;
  free(post->attachments);
  free(post->body);
  free(post->author_name);
}

PostV *new_post_v(void) {
  PostV *ret = malloc(sizeof(PostV));
  init_post_v(ret);
  return ret;
}

void ctr_post_v(PostV *post, _id id, _id related_post, _id author,
                time_t post_date, char *body, char *attachments,
                char *author_name) {
  post->id = id;
  post->related_post = related_post;
  post->author = author;
  post->post_date = post_date;
  assign_text(&post->body, body);
  assign_text(&post->attachments, attachments);
  assign_text(&post->author_name, author_name);
}

void delete_post_v(PostV *post) {
  finalize_post_v(post);
  free(post);
}

int find_post_by_id(sqlite3 *db, _id id, PostV *res) {
  int found_users;
  def_stmt("select "
           "count(p.rowid), p.id, related_post, author_id, "
           "post_date, body, attachments, s.name "
           "from Post p "
           "left join User s on author_id = s.id "
           "where p.id = @id");

  bind_id_v("@id", id);

  sqlite3_step(q);
  found_users = col_int(0);
  if (found_users == 0) {
    sqlite3_reset(q);
    return 0;
  }

  ctr_post_v(res, col_id(1), col_id(2), col_id(3), col_int64(4), col_text(5),
             col_text(6), col_text(7));

  sqlite3_reset(q);
  return found_users;
}

int read_responces(sqlite3 *db, _id post, PostV *res, int limit) {
  int received = 0;
  def_stmt("select "
           "s.name, p.id, related_post,author_id, post_date, body, attachments "
           "from Post p "
           "left join User s on author_id = s.id "
           "where related_post = @post "
           "order by post_date limit @limit");

  bind_id_v("@post", post);
  bind_id_v("@limit", limit);

  while (sqlite3_step(q) == SQLITE_ROW)
    ctr_post_v(&res[received++], col_id(1), col_id(2), col_id(3), col_int64(4),
               col_text(5), col_text(6), col_text(0));

  sqlite3_reset(q);
  return received;
}
int read_user_wall(sqlite3 *db, _id user, PostV *res, int limit) {
  int received = 0;
  def_stmt("select "
           "s.name, p.id, related_post,author_id, post_date, body, attachments "
           "from Post p "
           "left join User s on author_id = s.id "
           "where related_post = @wall and author_id = @user "
           "order by post_date limit @limit");

  bind_id_v("@wall", WALL_POST);
  bind_id_v("@user", user);
  bind_id_v("@limit", limit);

  while (sqlite3_step(q) == SQLITE_ROW) {
    ctr_post_v(&res[received++], col_id(1), col_id(2), col_id(3), col_int64(4),
               col_text(5), col_text(6), col_text(0));
  }

  sqlite3_reset(q);
  return received;
}

UserV *new_user_v(void) {
  UserV *ret = malloc(sizeof(UserV));
  init_user_v(ret);
  return ret;
}

void init_user_v(UserV *u) {
  u->details = NULL;
  u->name = NULL;
  u->password = NULL;
}

void finalize_user_v(UserV *u) {
  if (u == NULL)
    return;
  free(u->details);
  free(u->name);
  free(u->password);
}

void ctr_user_v(UserV *u, _id id, char *name, char *password, char *details) {
  u->id = id;
  assign_text(&u->name, name);
  assign_text(&u->password, password);
  assign_text(&u->details, details);
}

void delete_user_v(UserV *u) {
  if (u == NULL)
    return;
  finalize_user_v(u);
  free(u);
}

static int map_users(sqlite3_stmt *q, UserV *res) {
  int received = 0;
  while (sqlite3_step(q) == SQLITE_ROW) {
    ctr_user_v(&res[received++], col_id(0), col_text(1), col_text(2),
               col_text(3));
  }
  return received;
}

int find_users(sqlite3 *db, char *name, UserV *res, int limit) {
  char *like_wildcard = malloc(strlen(name) + 2 + 1);
  snprintf(like_wildcard, strlen(name) + 2 + 1, "%%%s%%", name); //%% escapes %
  def_stmt("select id, name, password, details "
           "from User where name like @name limit @limit");
  bind_text_v("@name", like_wildcard);
  bind_int_v("@limit", limit);

  int rec = map_users(q, res);
  sqlite3_reset(q);
  free(like_wildcard);
  return rec;
}

int find_user_by_id(sqlite3 *db, _id id, UserV *res) {
  int found_users;
  def_stmt("select count(rowid), name, password, details "
           "from User where id = @id");

  bind_id_v("@id", id);

  sqlite3_step(q);

  found_users = col_int(0);
  if (found_users == 0) {
    sqlite3_reset(q);
    return 0;
  }
  ctr_user_v(res, id, col_text(1), col_text(2), col_text(3));

  sqlite3_reset(q);
  return found_users;
}

int list_friends(sqlite3 *db, _id user, UserV *res, int limit) {
  int received = 0;
  def_stmt("select user_b, u.name, u.password, u.details "
           "from Friends "
           "left join User u on user_b = u.id "
           "where user_a = @id limit @limit");

  bind_id_v("@id", user);
  bind_int_v("@limit", limit);

  while (sqlite3_step(q) == SQLITE_ROW)
    ctr_user_v(&res[received++], col_id(0), col_text(1), col_text(2),
               col_text(3));

  sqlite3_reset(q);
  return received;
}

int list_invites(sqlite3 *db, _id user, UserV *res, int limit) {
  int received = 0;
  def_stmt("select inviter, u.name, u.password, u.details "
           "from FriendInvite "
           "left join User u on inviter = u.id "
           "where invited = @id limit @limit");

  bind_id_v("@id", user);
  bind_int_v("@limit", limit);

  while (sqlite3_step(q) == SQLITE_ROW)
    ctr_user_v(&res[received++], col_id(0), col_text(1), col_text(2),
               col_text(3));

  sqlite3_reset(q);
  return received;
}
