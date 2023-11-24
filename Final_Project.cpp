#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

struct Schedule {
    std::string date;
    std::string time;
};

enum PaymentMethod {
    CASH,
    DIGITAL_BANK,
    BANK
};

struct Payment {
    double amount;
    PaymentMethod method;
};

struct Doctor {
    std::string name;
    std::string specialty;
    std::vector<std::string> symptoms;
    Schedule schedule;
    double fee;
};

struct Patient {
    std::string name;
    std::string birthdate;
    std::string gender;
    std::string address;
    std::string contactDetails;
    std::string reason;
    std::string assignedDoctor;
    std::string status;
};

class QueueSystem {
private:
    std::vector<Patient> queue;
    std::vector<Doctor> doctors;

public:
    void addPatient(const Patient& patient) {
        queue.push_back(patient);
        std::cout << patient.name << " added to the queue." << std::endl;
    }

    void processPatient() {
        if (!queue.empty()) {
            Patient nextPatient = queue.front();
            queue.erase(queue.begin());

            if (!doctors.empty()) {
                auto it = std::find_if(doctors.begin(), doctors.end(),
                                       [&](const Doctor& doctor) {
                                           // Check for partial match or additional keywords
                                           return std::any_of(doctor.symptoms.begin(), doctor.symptoms.end(),
                                                              [&](const std::string& symptom) {
                                                                  return nextPatient.reason.find(symptom) != std::string::npos;
                                                              });
                                       });

                if (it != doctors.end()) {
                    Doctor& assignedDoctor = *it;
                    nextPatient.assignedDoctor = assignedDoctor.name;

                    std::cout << "Processing patient: " << nextPatient.name
                              << " - Doctor: " << nextPatient.assignedDoctor
                              << " - Specialty: " << assignedDoctor.specialty << std::endl;

                    std::cout << "Doctor's Fee: $" << assignedDoctor.fee << std::endl;

                    Payment payment = processPayment(assignedDoctor.fee);

                    std::cout << "Payment Details - Amount: $" << payment.amount
                              << " Method: " << paymentMethodToString(payment.method) << std::endl;

                    updatePatientStatus(nextPatient);

                    doctors.erase(it);
                } else {
                    nextPatient.assignedDoctor = "No doctor available for the specialty: " + nextPatient.reason;
                }
            } else {
                nextPatient.assignedDoctor = "No doctor available";
            }

            std::cout << "Processing patient: " << nextPatient.name
                      << " - Doctor: " << nextPatient.assignedDoctor << std::endl;
        } else {
            std::cout << "Queue is empty." << std::endl;
        }
    }

    Payment processPayment(double amount) {
        Payment payment;

        std::cout << "Select payment method:\n";
        std::cout << "1. Cash\n2. Digital Bank\n3. Bank\n";
        std::cout << "Selected payment method: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                payment.method = CASH;
                break;
            case 2:
                payment.method = DIGITAL_BANK;
                break;
            case 3:
                payment.method = BANK;
                break;
            default:
                std::cout << "Invalid payment method. Defaulting to Cash.\n";
                payment.method = CASH;
        }

        std::cout << "Enter the amount: $";
        std::cin >> payment.amount;

        return payment;
    }

    std::string paymentMethodToString(PaymentMethod method) {
        switch (method) {
            case CASH:
                return "Cash";
            case DIGITAL_BANK:
                return "Digital Bank";
            case BANK:
                return "Bank";
            default:
                return "Unknown";
        }
    }

    void updatePatientStatus(Patient& patient) {
        std::cout << "Update patient status (e.g., Recovered, In Progress, etc.): ";
        std::cin.ignore();
        std::getline(std::cin, patient.status);
        std::cout << "Patient status updated to: " << patient.status << std::endl;
    }

    void displayQueue() {
        std::cout << "Current Queue:" << std::endl;
        for (const auto& patient : queue) {
            std::cout << "Name: " << patient.name << " - Reason: " << patient.reason << std::endl;
        }
    }

    void addDoctor(const Doctor& doctor) {
        doctors.push_back(doctor);
        std::cout << doctor.name << " added as a doctor." << std::endl;
    }

    void displayDoctors() {
        if (doctors.empty()) {
            std::cout << "No doctors available." << std::endl;
        } else {
            std::cout << "Available Doctors:" << std::endl;
            displayDoctorHeader();
            for (const auto& doctor : doctors) {
                displayDoctor(doctor);
            }
            displayDoctorFooter();
        }
    }

    void displayDoctorHeader() {
        std::cout << "*********************************************************************************************************" << std::endl;
        std::cout << std::setw(20) << std::left << "Doctor Name"
                  << std::setw(20) << std::left << "Specialty"
                  << std::setw(20) << std::left << "Schedule"
                  << std::setw(35) << std::right << "Fee" << std::endl;
        std::cout << "*********************************************************************************************************" << std::endl;
    }

    void displayDoctor(const Doctor& doctor) {
        std::cout << std::setw(20) << std::left << doctor.name
                  << std::setw(20) << std::left << doctor.specialty
                  << std::setw(20) << std::left << doctor.schedule.date << " Time: " << doctor.schedule.time
                  << std::setw(10) << std::right << "$" << doctor.fee << std::endl;

        // Display symptoms based on doctor's specialty
        std::cout << "Specialty Symptoms:\n";
        for (const auto& symptom : doctor.symptoms) {
            std::cout << " - " << symptom << "\n";
        }

        std::cout << std::endl;
    }

    void displayDoctorFooter() {
        std::cout << "*********************************************************************************************************" << std::endl;
    }

    void searchPatient(const std::string& patientName) {
        auto it = std::find_if(queue.begin(), queue.end(),
                               [&](const Patient& patient) {
                                   return patient.name == patientName;
                               });

        if (it != queue.end()) {
            const Patient& foundPatient = *it;
            std::cout << "Patient found:\n";
            displayPatient(foundPatient);
        } else {
            std::cout << "Patient not found.\n";
        }
    }

    void displayPatient(const Patient& patient) {
        std::cout << "Name: " << patient.name << "\n"
                  << "Birthdate: " << patient.birthdate << "\n"
                  << "Gender: " << patient.gender << "\n"
                  << "Address: " << patient.address << "\n"
                  << "Contact Details: " << patient.contactDetails << "\n"
                  << "Reason: " << patient.reason << "\n"
                  << "Assigned Doctor: " << patient.assignedDoctor << "\n"
                  << "Status: " << patient.status << "\n";
    }
};

int main() {
    QueueSystem queueSystem;
    std::string nameToSearch;

    Doctor doctor1{"Dr. Smith", "Cardiologist", {"Chest pain or discomfort", "Shortness of breath", "Irregular heartbeats (arrhythmias)", "High blood pressure", "Fatigue or weakness", "Dizziness or fainting", "Swelling in the legs, ankles, or abdomen", "High cholesterol"}, {"Mon-Wed", "7:00am - 5:00pm"}, 150.0};
    Doctor doctor2{"Dr. Johnson", "Pediatrician", {"Fever", "Cough and cold symptoms", "Ear infections", "Respiratory issues", "Developmental concerns", "Behavioral issues", "Immunization and vaccination", "Growth and nutrition concerns"}, {"Tue-Thurs", "7:00am - 5:00pm"}, 120.0};
    Doctor doctor3{"Dr. Rob", "Surgeon", {"Acute injuries (such as fractures)", "Abdominal pain", "Tumors or growths", "Infections requiring surgical intervention", "Need for elective surgery (e.g., hernia repair, gallbladder removal)", "Trauma or injuries requiring surgical intervention", "Gastrointestinal issues", "Orthopedic issues (e.g., joint problems)"}, {"Fri-Sat", "7:00am - 5:00pm"}, 140.0};
    queueSystem.addDoctor(doctor1);
    queueSystem.addDoctor(doctor2);
    queueSystem.addDoctor(doctor3);

    int choice;
    do {
        std::cout << "Hospital Queue System" << std::endl;
        std::cout << "1. Display Doctors\n2. Add Patient\n3. Process Patient\n4. Display Queue\n5. Search Patient\n6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                queueSystem.displayDoctors();
                break;
            case 2: {
                std::string name, birthdate, gender, address, contactDetails, reason;
                std::cout << "Enter patient's name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter patient's birthdate: ";
                std::getline(std::cin, birthdate);
                std::cout << "Enter patient's gender: ";
                std::getline(std::cin, gender);
                std::cout << "Enter patient's address: ";
                std::getline(std::cin, address);
                std::cout << "Enter reason for visit: ";
                std::getline(std::cin, reason);

                Patient patient{name, birthdate, gender, address, contactDetails, reason, "", "Pending"};
                queueSystem.addPatient(patient);
                break;
            }
            case 3:
                queueSystem.processPatient();
                break;
            case 4:
                queueSystem.displayQueue();
                break;
            case 5:
                std::cout << "Enter patient name to search: ";
                std::cin.ignore();
                std::getline(std::cin, nameToSearch);
                queueSystem.searchPatient(nameToSearch);
                break;
            case 6:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 6);

    return 0;
}