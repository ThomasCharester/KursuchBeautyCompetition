#pragma once
#ifndef PARTICIPANT
#define PARTICIPANT

#include <string>
#include <vector>
#include <fstream>

struct Participant {
	struct Rating {
		int rate = 0;
		vector<string> rated;
		vector<int> ratings;

		void addRating(string login, int rating) {
			for (int i = 0; i < rated.size(); i++)
				if (rated.at(i) == login) {
					ratings.at(i) = rating;
					return;
				}
			rated.emplace_back(login);
			ratings.emplace_back(rating);
		}
		void calculateRating() {
			if (ratings.empty()) return;
			rate = 0;
			for (int r : ratings)
				rate += r;
			rate /= ratings.size();
		}

		void addToFile() const
		{
			fstream file("participants.txt", ios::app);
			for (int i = 0; i < rated.size(); i++)
				file << "r " << rated.at(i) << ' ' << ratings.at(i) << '\n';

			file.close();
		}
	};

	Rating rating;

	string name = "";
	string surName = "";
	string country = "";
	int age = 0;
	int weight = 0;
	int height = 0;
	bool isParticipant = true;
	Participant() {};
	Participant(string name, string surName, string country, int age, int height, int weight, int rate, bool isParticipant) {
		this->name = name;
		this->surName = surName;
		this->country = country;
		this->age = age;
		this->height = height;
		this->weight = weight;
		this->rating.rate = rate;
		this->isParticipant = isParticipant;
	}
	void addToFile() const
	{
		fstream file("participants.txt", ios::app);
		file << "p " << name << ' ' << surName << ' ' << country << ' ' << age << ' ' << height << ' ' << weight << ' ' << rating.rate << ' ' << isParticipant << '\n';

		rating.addToFile();

		file.close();
	}
	static bool byRate(const Participant* pt1, const Participant* pt2) {
		return pt1->rating.rate < pt2->rating.rate;
	}
	static bool byHeight(const Participant* pt1, const Participant* pt2) {
		return pt1->height < pt2->height;
	}
	static bool byAge(const Participant* pt1, const Participant* pt2) {
		return pt1->age < pt2->age;
	}
	static bool byWeight(const Participant* pt1, const Participant* pt2) {
		return pt1->weight < pt2->weight;
	}
	static bool byCountry(const Participant* pt1, const Participant* pt2) {
		return pt1->country.compare(pt2->country) < 0;
	}
};
#endif // !PARTICIPANT