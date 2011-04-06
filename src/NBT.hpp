#ifndef NBT_HPP
#define NBT_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace nbt{
	/*!
	 * \brief The base tag
	 *
	 * The base tag from which all other tags are derived.
	 */
	class Tag{
		private:
			std::string TagName;
		public:
			/*!
			 * \param Name The name of the tag, defaults to empty
			 *
			 * Construct a tag with a name.
			 */
			Tag(const std::string &Name = "");

			/*!
			 * \return The name of the tag
			 *
			 * Retrieve the name of the tag.
			 */
			const std::string &GetName() const;

			/*!
			 * \return The tag type
			 *
			 * Retrieve the type of the tag.
			 */
			virtual int8_t GetType() const;

			/*!
			 * \param Stream The stream from which the tag will be loaded
			 * \param LoadName Will the tag load a name from the stream?
			 * \return True if successful, false otherwise
			 *
			 * Load a tag from a stream, discarding the characters it uses.
			 */
			virtual bool Load(std::istream &Stream, bool LoadName = true) = 0;

			/*!
			 * \param Stream The stream in which the tag will be saved
			 * \param WriteName Will the tag write a name into the stream?
			 * \return True if successful, false otherwise
			 *
			 * Save a tag into a stream.
			 */
			virtual bool Save(std::ostream &Stream, bool WriteName = true) = 0;

			/*!
			 * \param Name The new name of the tag
			 *
			 * Change the name of the tag to the one provided.
			 */
			void SetName(const std::string &Name);

			/*!
			 * Destroy the tag.
			 */
			virtual ~Tag();
	};

	/*!
	 * \brief Tag containing a single byte
	 *
	 * Tag with a type of 1, stores a single byte.
	 */
	class Byte : public Tag{
		private:
			int8_t Payload;
		public:
			/*!
			 * \param Name The name of the byte, defaults to empty
			 * \param Value The payload of the byte, defaults to 0
			 *
			 * Construct a byte with a name and a payload.
			 */
			Byte(const std::string &Name = "", const int8_t &Value = 0);

			/*!
			 * \return The byte's payload
			 *
			 * Retrieve the payload of the byte.
			 */
			const int8_t &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the byte
			 *
			 * Change the payload of the byte to the one provided.
			 */
			void SetPayload(const int8_t &Value);

			/*!
			 * Destroy the byte.
			 */
			~Byte();
	};

	/*!
	 * \brief Tag containing a short (two bytes)
	 *
	 * Tag with a type of 2, stores a short (two bytes).
	 */
	class Short : public Tag{
		private:
			int16_t Payload;
		public:
			/*!
			 * \param Name The name of the short, defaults to empty
			 * \param Value The payload of the short, defaults to 0
			 *
			 * Construct a short with a name and a payload.
			 */
			Short(const std::string &Name = "", const int16_t &Value = 0);

			/*!
			 * \return The short's payload
			 *
			 * Retrieve the payload of the short.
			 */
			const int16_t &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the short
			 *
			 * Change the payload of the short to the one provided.
			 */
			void SetPayload(const int16_t &Value);

			/*!
			 * Destroy the short.
			 */
			~Short();
	};

	/*!
	 * \brief Tag containing an int (four bytes)
	 *
	 * Tag with a type of 3, stores an int (four bytes).
	 */
	class Int : public Tag{
		private:
			int32_t Payload;
		public:
			/*!
			 * \param Name The name of the int, defaults to empty
			 * \param Value The payload of the int, defaults to 0
			 *
			 * Construct an int with a name and a payload.
			 */
			Int(const std::string &Name = "", const int32_t &Value = 0);

			/*!
			 * \return The int's payload
			 *
			 * Retrieve the payload of the int.
			 */
			const int32_t &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the int
			 *
			 * Change the payload of the int to the one provided.
			 */
			void SetPayload(const int32_t &Value);

			/*!
			 * Destroy the int.
			 */
			~Int();
	};

	/*!
	 * \brief Tag containing a long (eight bytes)
	 *
	 * Tag with a type of 4, stores a long (eight bytes).
	 */
	class Long : public Tag{
		private:
			int64_t Payload;
		public:
			/*!
			 * \param Name The name of the long, defaults to empty
			 * \param Value The payload of the long, defaults to 0
			 *
			 * Construct a long with a name and a payload.
			 */
			Long(const std::string &Name = "", const int64_t &Value = 0);

			/*!
			 * \return The long's payload
			 *
			 * Retrieve the payload of the long.
			 */
			const int64_t &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the long
			 *
			 * Change the payload of the long to the one provided.
			 */
			void SetPayload(const int64_t &Value);

			/*!
			 * Destroy the long.
			 */
			~Long();
	};

	/*!
	 * \brief Tag containing a float (four bytes)
	 *
	 * Tag with a type of 5, stores a float (four bytes).
	 */
	class Float : public Tag{
		private:
			float Payload;
		public:
			/*!
			 * \param Name The name of the float, defaults to empty
			 * \param Value The payload of the float, defaults to 0
			 *
			 * Construct a float with a name and a payload.
			 */
			Float(const std::string &Name = "", const float &Value = 0);

			/*!
			 * \return The float's payload
			 *
			 * Retrieve the payload of the float.
			 */
			const float &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the float
			 *
			 * Change the payload of the float to the one provided.
			 */
			void SetPayload(const float &Value);

			/*!
			 * Destroy the float.
			 */
			~Float();
	};

	/*!
	 * \brief Tag containing a double (eight bytes)
	 *
	 * Tag with a type of 7, stores a double (eight bytes).
	 */
	class Double : public Tag{
		private:
			double Payload;
		public:
			/*!
			 * \param Name The name of the double, defaults to empty
			 * \param Value The payload of the double, defaults to 0
			 *
			 * Construct a double with a name and a payload.
			 */
			Double(const std::string &Name = "", const double &Value = 0);

			/*!
			 * \return The double's payload
			 *
			 * Retrieve the payload of the double.
			 */
			const double &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the double
			 *
			 * Change the payload of the double to the one provided.
			 */
			void SetPayload(const double &Value);

			/*!
			 * Destroy the double.
			 */
			~Double();
	};

	/*!
	 * \brief Tag containing an array of bytes
	 *
	 * Tag with a type of 7, stores an array of bytes.
	 */
	class ByteArray : public Tag{
		private:
			std::vector<char> Payload;
		public:
			/*!
			 * \param Name The name of the byte array, defaults to empty
			 * \param Value The payload of the byte array, defaults to empty
			 *
			 * Construct an array of bytes with a name and a payload.
			 */
			ByteArray(const std::string &Name = "", const std::vector<char> &Value = std::vector<char>());

			/*!
			 * \return The byte array's payload
			 *
			 * Retrieve the payload of the byte array.
			 */
			const std::vector<char> &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the byte array
			 *
			 * Change the payload of the byte array to the one provided.
			 */
			void SetPayload(const std::vector<char> &Value);

			/*!
			 * Destroy the byte array.
			 */
			~ByteArray();
	};

	/*!
	 * \brief Tag containing a string
	 *
	 * Tag with a type of 8, stores a string.
	 */
	class String : public Tag{
		private:
			std::string Payload;
		public:
			/*!
			 * \param Name The name of the string, defaults to empty
			 * \param Value The payload of the string, defaults to empty
			 *
			 * Construct a string with a name and a payload.
			 */
			String(const std::string &Name = "", const std::string &Value = "");

			/*!
			 * \return The string's payload
			 *
			 * Retrieve the payload of the string.
			 */
			const std::string &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the string
			 *
			 * Change the payload of the string to the one provided.
			 */
			void SetPayload(const std::string &Value);

			/*!
			 * Destroy the string.
			 */
			~String();
	};

	/*!
	 * \brief Tag containing a list of tags
	 *
	 * Tag with a type of 9, stores a list of unnamed tags of one type.
	 */
	class List : public Tag{
		private:
			std::vector<std::shared_ptr<Tag>> Payload;
			int8_t Type;
			void ClearMismatchedTypes();
		public:
			/*!
			 * \param Name The name of the list, defaults to empty
			 * \param Value The payload of the list, defaults to empty
			 *
			 * Construct a list with a name and a payload.
			 */
			List(const std::string &Name = "", const std::vector<std::shared_ptr<Tag>> &Value = std::vector<std::shared_ptr<Tag>>());

			/*!
			 * \return The list's payload
			 *
			 * Retrieve the payload of the list.
			 */
			const std::vector<std::shared_ptr<Tag>> &GetPayload() const;

			int8_t GetTagType() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the list
			 *
			 * Change the payload of the list to the one provided.
			 */
			void SetPayload(const std::vector<std::shared_ptr<Tag>> &Value);

			/*!
			 * Destroy the list.
			 */
			~List();
	};

	/*!
	 * \brief Tag containing a list of tags
	 *
	 * Tag with a type of 10, stores a list of tags of any type.
	 */
	class Compound : public Tag{
		private:
			std::vector<std::shared_ptr<Tag>> Payload;
		public:
			/*!
			 * \param Name The name of the compound, defaults to empty
			 * \param Value The payload of the compound, defaults to empty
			 *
			 * Construct a compound with a name and a payload.
			 */
			Compound(const std::string &Name = "", const std::vector<std::shared_ptr<Tag>> &Value = std::vector<std::shared_ptr<Tag>>());

			/*!
			 * \param Name The name of the tag to locate
			 * \return The first tag with a given name
			 *
			 * Locate the first tag in the compound with a given name.
			 */
			std::vector<std::shared_ptr<Tag>>::iterator FirstChild(std::string Name);

			/*!
			 * \return The compound's payload
			 *
			 * Retrieve the payload of the compound.
			 */
			const std::vector<std::shared_ptr<Tag>> &GetPayload() const;

			int8_t GetType() const;

			bool Load(std::istream &Stream, bool LoadName = true);

			/*!
			 * \param Name The name of the tag to locate
			 * \param Current The tag to start the search from
			 * \return The next tag with a given name
			 *
			 * Locate the next tag in the compound with a given name.
			 */
			std::vector<std::shared_ptr<Tag>>::iterator NextChild(std::string Name, std::vector<std::shared_ptr<Tag>>::iterator Current);

			bool Save(std::ostream &Stream, bool WriteName = true);

			/*!
			 * \param Value The new payload of the compound
			 *
			 * Change the payload of the compound to the one provided.
			 */
			void SetPayload(const std::vector<std::shared_ptr<Tag>> &Value);

			/*!
			 * Destroy the compound.
			 */
			~Compound();
	};

	/*!
	 * \brief NBT file
	 *
	 * An NBT file which can be loaded or saved. Contains a single root compound which can be retrieved and modified.
	 */
	class File{
		private:
			/*!
			 * The file's root.
			 */
			std::shared_ptr<Compound> Root;
		public:
			/*!
			 * Construct an empty file
			 */
			File();

			/*!
			 * \param Filename The name of the file to load
			 *
			 * Construct a file and load it from the given filename.
			 */
			File(const std::string &Filename);

			/*!
			 * Retrieve the file's root compound.
			 */
			std::shared_ptr<Compound> &GetRoot();

			/*!
			 * \param Filename The name of the file to load
			 * \return True if the file was successfully loaded, false otherwise
			 *
			 * Load an NBT file from the given filename.
			 */
			bool Load(const std::string &Filename);

			/*!
			 * \param Filename The name of the file to save to
			 * \return True if the file was successfully saved, false otherwise
			 *
			 * Save an NBT file to the given filename.
			 */
			bool Save(const std::string &Filename);

			/*!
			 * \param NewRoot Shared pointer to the new root
			 *
			 * Set the file's root to the one provided.
			 */
			void SetRoot(const std::shared_ptr<Compound> &NewRoot);

			/*!
			 * Destroy the file.
			 */
			~File();
	};
}

#endif
