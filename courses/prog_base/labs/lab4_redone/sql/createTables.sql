create table User(
  id integer primary key autoincrement not null,
  name text not null,
  password text not null,
  details text
);

create table Post(
  id integer primary key autoincrement not null,
  author_id integer not null,
  related_post integer not null,
  post_date datetime default (strftime('%s', 'now')) not null,
  body text,
  attachments text,
  foreign key (author_id) references User(id),
  foreign key (related_post) references Post(id)
);

create table Like(
  id integer primary key autoincrement not null,
  liker_id integer not null,
  post_id integer not null,
  foreign key (liker_id) references User(id),
  foreign key (post_id) references Post(id)
);

create table Friends(
  user_a integer not null,
  user_b integer not null,
  foreign key (user_a) references User(id),
  foreign key (user_b) references User(id)
);

create table FriendInvite(
  inviter integer not null,
  invited integer not null,
  foreign key(inviter) references User(id),
  foreign key(invited) references User(id)
);

create table Message(
  id integer primary key autoincrement not null,
  sender_id integer not null,
  receiver_id integer not null,
  post_date datetime default (strftime('%s', 'now')) not null,
  body text,
  attachments text,
  foreign key(sender_id) references User(id),
  foreign key(receiver_id) references User(id)
);
